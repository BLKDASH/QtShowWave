#include "keywordhighlighter.h"

// 辅助函数：添加关键词高亮规则
void KeywordHighlighter::addKeywordRule(const QString &pattern, const QColor &color, 
                                         bool bold, bool caseInsensitive)
{
    HighlightRule rule;
    QTextCharFormat format;
    format.setForeground(color);
    if (bold) {
        format.setFontWeight(QFont::Bold);
    }
    
    QRegularExpression::PatternOptions options = QRegularExpression::NoPatternOption;
    if (caseInsensitive) {
        options = QRegularExpression::CaseInsensitiveOption;
    }
    rule.pattern = QRegularExpression(pattern, options);
    rule.format = format;
    m_rules.append(rule);
}

/**
 * @brief KeywordHighlighter 构造函数
 * 
 * 初始化高亮规则，支持多种日志级别和常见模式。
 * Requirements: 3.2, 3.5
 */
KeywordHighlighter::KeywordHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_enabled(true)
{
    // === 日志级别关键词 ===
    // DEBUG/TRACE/D: - 灰色
    addKeywordRule("\\b(debug|trace|verbose)\\b", QColor("#808080"), false, true);
    addKeywordRule("^\\s*[DdVv]:", QColor("#808080"), false, false);  // D: V: 开头
    
    // INFO/I: - 蓝色
    addKeywordRule("\\b(info|notice)\\b", QColor("#0066CC"), false, true);
    addKeywordRule("^\\s*[Ii]:", QColor("#0066CC"), false, false);  // I: 开头
    
    // WARNING/WARN/W: - 橙色
    addKeywordRule("\\b(warning|warn)\\b", QColor("#FF9900"), false, true);
    addKeywordRule("^\\s*[Ww]:", QColor("#FF9900"), false, false);  // W: 开头
    
    // ERROR/ERR/FAIL/E: - 红色
    addKeywordRule("\\b(error|err|fail|failed|failure)\\b", QColor("#CC0000"), false, true);
    addKeywordRule("^\\s*[Ee]:", QColor("#CC0000"), false, false);  // E: 开头
    
    // FATAL/CRITICAL/F: - 深红色
    addKeywordRule("\\b(fatal|critical|panic)\\b", QColor("#990000"), false, true);
    addKeywordRule("^\\s*[Ff]:", QColor("#990000"), false, false);  // F: 开头
    
    // SUCCESS/OK/PASS - 绿色
    addKeywordRule("\\b(success|ok|pass|passed|done|complete|completed)\\b", QColor("#00AA00"), false, true);
    
    // SYSINFO - 绿色
    addKeywordRule("\\bsysinfo\\b", QColor("#00AA00"), false, true);
    
    // === 常见日志格式前缀 ===
    // [INFO] [WARN] [ERROR] 等方括号格式
    addKeywordRule("\\[DEBUG\\]|\\[TRACE\\]|\\[VERBOSE\\]", QColor("#808080"), false, true);
    addKeywordRule("\\[INFO\\]|\\[NOTICE\\]", QColor("#0066CC"), false, true);
    addKeywordRule("\\[WARN\\]|\\[WARNING\\]", QColor("#FF9900"), false, true);
    addKeywordRule("\\[ERROR\\]|\\[ERR\\]|\\[FAIL\\]", QColor("#CC0000"), false, true);
    addKeywordRule("\\[FATAL\\]|\\[CRITICAL\\]", QColor("#990000"), false, true);
    
    // === 特殊格式 ===
    // 时间戳 (HH:mm:ss.zzz >>) - 灰色
    addKeywordRule("\\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\s*>>", QColor("#808080"), false, false);
    
    // 十六进制数 (0x...) - 紫色
    addKeywordRule("\\b0x[0-9A-Fa-f]+\\b", QColor("#9932CC"), false, false);
    
    // 数字 - 青色 (可选，如果觉得太花哨可以注释掉)
    // addKeywordRule("\\b\\d+\\b", QColor("#008B8B"), false, false);
    
    // SEND >> 前缀 - 灰色斜体
    HighlightRule sendRule;
    QTextCharFormat sendFormat;
    sendFormat.setForeground(QColor("#666666"));
    sendFormat.setFontItalic(true);
    sendRule.pattern = QRegularExpression("SEND\\s*>>");
    sendRule.format = sendFormat;
    m_rules.append(sendRule);
}

/**
 * @brief 设置高亮功能是否启用
 * @param enabled true 启用高亮，false 禁用
 * Requirements: 3.3
 */
void KeywordHighlighter::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        // 触发重新高亮整个文档
        rehighlight();
    }
}

/**
 * @brief 检查高亮功能是否启用
 * @return true 如果高亮已启用
 */
bool KeywordHighlighter::isEnabled() const
{
    return m_enabled;
}

/**
 * @brief 高亮文本块
 * 
 * 遍历所有高亮规则，对匹配的关键词应用相应的颜色格式。
 * 如果高亮功能被禁用，则不应用任何格式。
 * 
 * @param text 要处理的文本块
 * Requirements: 3.2, 3.5
 */
void KeywordHighlighter::highlightBlock(const QString &text)
{
    // 如果高亮功能被禁用，不应用任何格式
    if (!m_enabled) {
        return;
    }

    // 遍历所有高亮规则
    for (const HighlightRule &rule : m_rules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
