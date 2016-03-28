#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QThread>
#include <QMessageBox>

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	private slots:
		void on_inputBrowseButton_clicked();
		void on_OutputBrowseButton_clicked();
		void on_convertButton_clicked();
		void on_cancelButton_clicked();
		void on_videoCodecSelect_currentTextChanged(const QString &arg1);
		void on_inputLineEdit_textChanged(const QString &arg1);
		void on_outputLineEdit_textChanged(const QString &arg1);
		void on_actionAbout_triggered();
		void on_actionExit_triggered();

		void processStarted();
		void processFinished();
		void readyReadStandardOutput();

		void updateCommand();

	private:
		Ui::MainWindow *ui;
		QString inputFilename;
		QString outputFilename;
		QStringList arguments;
		QProcess *transcodingProcess;
};

#endif // MAINWINDOW_H
