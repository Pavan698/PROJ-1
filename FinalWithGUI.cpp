#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QStringListModel>

class SpellCheckerWidget : public QWidget {
    Q_OBJECT

private:
    Trie wordTrie;
    QLineEdit* inputField;
    QTextEdit* outputField;
    QStringListModel* suggestionsModel;

public:
    SpellCheckerWidget(QWidget* parent = nullptr) : QWidget(parent) {
        loadDictionaryFromFile(wordTrie, "dictionary.txt");

        inputField = new QLineEdit(this);
        outputField = new QTextEdit(this);
        suggestionsModel = new QStringListModel(this);

        QPushButton* checkButton = new QPushButton("Check Spelling", this);
        connect(checkButton, &QPushButton::clicked, this, &SpellCheckerWidget::performSpellCheck);

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(inputField);
        layout->addWidget(checkButton);
        layout->addWidget(outputField);

        setLayout(layout);
    }

public slots:
    void performSpellCheck() {
        QString inputWord = inputField->text();

        if (inputWord == "exit") {
            qApp->quit();
            return;
        }

        if (wordTrie.search(inputWord.toStdString())) {
            outputField->setText("The word is spelled correctly.");
        } else {
            std::vector<std::string> suggestions = wordTrie.suggest(inputWord.toStdString());

            if (suggestions.empty()) {
                outputField->setText("No suggestions found.");
            } else {
                QStringList suggestionList;
                for (const std::string& suggestion : suggestions) {
                    suggestionList.append(QString::fromStdString(suggestion));
                }
                suggestionsModel->setStringList(suggestionList);
                outputField->setModel(suggestionsModel);
            }
        }
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    SpellCheckerWidget spellChecker;
    spellChecker.show();

    return app.exec();
}

#include "main.moc"
