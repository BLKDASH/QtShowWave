#include "keywordhighlighter.h"

/**
 * @brief KeywordHighlighter 构造函数
 * 
 * 初始化高亮规则：
 * - info: 蓝色 (#0066CC)
 * - warning: 橙色 (#FF9900)
 * - error: 红色 (#CC0000)
 * - sysinfo: 绿色 (#00AA00)
 * - 时间戳: 灰色 (#808080)
 * 
 * 所有规则使用大小写不敏感匹配。
 * Requirements: 3.2, 3.5
 */
KeywordHighlighter::KeywordHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
    , m_enabled(true)
{
    HighlightRule rule;

    // info 关键词 - 蓝色
    QTextCharFormat infoFormat;
    infoFormat.setForeground(QColor("#0066CC"));
    rule.pattern = QRegularExpression(QStringLiteral("\\binfo\\b"),
                                      QRegularExpression::CaseInsensitiveOption);
    rule.format = infoFormat;
    m_rules.append(rule);

    // warning 关键词 - 橙色
    QTextCharFormat warningFormat;
    warningFormat.setForeground(QColor("#FF9900"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bwarning\\b"),
                                      QRegularExpression::CaseInsensitiveOption);
    rule.format = warningFormat;
    m_rules.append(rule);

    // error 关键词 - 红色
    QTextCharFormat errorFormat;
    errorFormat.setForeground(QColor("#CC0000"));
    rule.pattern = QRegularExpression(QStringLiteral("\\berror\\b"),
                                      QRegularExpression::CaseInsensitiveOption);
    rule.format = errorFormat;
    m_rules.append(rule);

    // sysinfo 关键词 - 绿色
    QTextCharFormat sysinfoFormat;
    sysinfoFormat.setForeground(QColor("#00AA00"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bsysinfo\\b"),
                                      QRegularExpression::CaseInsensitiveOption);
    rule.format = sysinfoFormat;
    m_rules.append(rule);

    // 时间戳 - 灰色 (格式: HH:mm:ss.zzz >>)
    QTextCharFormat timestampFormat;
    timestampFormat.setForeground(QColor("#808080"));  // 灰色
    rule.pattern = QRegularExpression(QStringLiteral("\\d{2}:\\d{2}:\\d{2}\\.\\d{3}\\s*>>"));
    rule.format = timestampFormat;
    m_rules.append(rule);
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
