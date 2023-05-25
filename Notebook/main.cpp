#include <QtWidgets>

class Notebook : public QWidget {
public:
    Notebook(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
    }

private:
    QVBoxLayout *layout;
    QTextEdit *noteTextEdit;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *newButton;
    QPushButton *openLastButton;
    QPushButton *openMultipleButton;

    void setupUI() {
        layout = new QVBoxLayout(this);
        noteTextEdit = new QTextEdit(this);
        saveButton = new QPushButton("Сохранить", this);
        loadButton = new QPushButton("Загрузить", this);
        newButton = new QPushButton("Новая запись", this);
        openLastButton = new QPushButton("Открыть последнюю запись", this);
        openMultipleButton = new QPushButton("Открыть множество записей", this);

        layout->addWidget(noteTextEdit);
        layout->addWidget(saveButton);
        layout->addWidget(loadButton);
        layout->addWidget(newButton);
        layout->addWidget(openLastButton);
        layout->addWidget(openMultipleButton);

        connect(saveButton, &QPushButton::clicked, this, &Notebook::saveNote);
        connect(loadButton, &QPushButton::clicked, this, &Notebook::loadNote);
        connect(newButton, &QPushButton::clicked, this, &Notebook::newNote);
        connect(openLastButton, &QPushButton::clicked, this, &Notebook::openLastNote);
        connect(openMultipleButton, &QPushButton::clicked, this, &Notebook::openMultipleNotes);

        setLayout(layout);
        setWindowTitle("Записная книжка");
    }

private slots:
    void saveNote() {
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить запись", "", "Текстовые файлы (*.txt)");

        if (!fileName.isEmpty()) {
            QFile file(fileName);

            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                stream << noteTextEdit->toPlainText();
                file.close();
                QMessageBox::information(this, "Успех", "Запись сохранена.");
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось сохранить запись.");
            }
        }
    }

    void loadNote() {
        QString fileName = QFileDialog::getOpenFileName(this, "Загрузить запись", "", "Текстовые файлы (*.txt)");

        if (!fileName.isEmpty()) {
            QFile file(fileName);

            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                noteTextEdit->setText(stream.readAll());
                file.close();
                QMessageBox::information(this, "Успех", "Запись загружена.");
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось загрузить запись.");
            }
        }
    }

    void newNote() {
        noteTextEdit->clear();
    }

    void openLastNote() {
        QString lastFileName = "last_note.txt"; // Путь к файлу последней записи
        QFile file(lastFileName);

        if (file.exists()) {
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream stream(&file);
                noteTextEdit->setText(stream.readAll());
                file.close();
                QMessageBox::information(this, "Успех", "Последняя запись загружена.");
            } else {
                QMessageBox::critical(this, "Ошибка", "Не удалось загрузить последнюю запись.");
            }
        } else {
            QMessageBox::information(this, "Информация", "Последняя запись не найдена.");
        }
    }

    void openMultipleNotes() {
        QStringList fileNames = QFileDialog::getOpenFileNames(this, "Открыть записи", "", "Текстовые файлы (*.txt)");

        if (!fileNames.isEmpty()) {
            foreach (QString fileName, fileNames) {
                QFile file(fileName);

                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream stream(&file);
                    QString noteText = stream.readAll();
                    file.close();

                    // Создание нового окна для каждой записи
                    QTextEdit *noteTextEdit = new QTextEdit();
                    noteTextEdit->setPlainText(noteText);
                    noteTextEdit->show();
                } else {
                    QMessageBox::critical(this, "Ошибка", "Не удалось загрузить запись.");
                }
            }

            QMessageBox::information(this, "Успех", "Записи загружены.");
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Notebook notebook;
    notebook.show();

    return app.exec();
}
