#ifndef KEYWORDHIGHLIGHTER_H
#define KEYWORDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QVector>

/**
 * @brief KeywordHighlighter - 关键词高亮器
 * 
 * 继承自 QSyntaxHighlighter，用于在 QTextDocument 中高亮显示
 * 特定关键词（info, warning, error）。
 * 
 * Requirements: 3.1, 3.2, 3.3
 */
class KeywordHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    /**
     * @brief 构造函数
     * @param parent 父 QTextDocument 对象
     */
    explicit KeywordHighlighter(QTextDocument *parent = nullptr);

    /**
     * @brief 设置高亮功能是否启用
     * @param enabled true 启用高亮，false 禁用
     * Requirements: 3.3
     */
    void setEnabled(bool enabled);

    /**
     * @brief 检查高亮功能是否启用
     * @return true 如果高亮已启用
     */
    bool isEnabled() const;

protected:
    /**
     * @brief 高亮文本块
     * 
     * 重写 QSyntaxHighlighter::highlightBlock，对文本中的
     * 关键词应用相应的颜色格式。
     * 
     * @param text 要处理的文本块
     * Requirements: 3.2, 3.5
     */
    void highlightBlock(const QString &text) override;

private:
    /**
     * @brief 高亮规则结构体
     * 
     * 定义关键词匹配模式和对应的文本格式。
     */
    struct HighlightRule {
        QRegularExpression pattern;  ///< 匹配模式（正则表达式）
        QTextCharFormat format;      ///< 文本格式（颜色等）
    };

    /**
     * @brief 添加关键词高亮规则
     * @param pattern 正则表达式模式
     * @param color 高亮颜色
     * @param bold 是否加粗
     * @param caseInsensitive 是否大小写不敏感
     */
    void addKeywordRule(const QString &pattern, const QColor &color, 
                        bool bold = false, bool caseInsensitive = true);

    QVector<HighlightRule> m_rules;  ///< 高亮规则列表
    bool m_enabled = true;           ///< 高亮启用状态
};

#endif // KEYWORDHIGHLIGHTER_H
