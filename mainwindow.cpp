#include "mainwindow.h"
#include "ui_mainwindow.h"

// Use globally installed ffmpeg binary if running on Linux, and use included ffmpeg.exe in the case of Windows
#if defined(Q_OS_LINUX)
QString ffmpegPath = "/usr/bin/ffmpeg";
#elif defined(Q_OS_WIN32)
QString ffmpegPath = "ffmpeg.exe";
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::MainWindow),
	transcodingProcess(new QProcess)
{
    ui->setupUi(this);
	this->on_videoCodecSelect_currentTextChanged(ui->videoCodecSelect->currentText());

	connect(this->transcodingProcess, SIGNAL(started()), this, SLOT(processStarted()));
	connect(this->transcodingProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
	connect(this->transcodingProcess, SIGNAL(finished(int)), this, SLOT(processFinished()));

	connect(ui->audioCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateCommand()));
	connect(ui->audioCodecSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCommand()));
	connect(ui->bitrateEdit, SIGNAL(textChanged(QString)), this, SLOT(updateCommand()));
	connect(ui->CRFSlider, SIGNAL(valueChanged(int)), this, SLOT(updateCommand()));
	connect(ui->speedSlider, SIGNAL(valueChanged(int)), this, SLOT(updateCommand()));
	connect(ui->tilecolumnsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateCommand()));
	connect(ui->frameparallelCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateCommand()));
	connect(ui->ColourspaceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCommand()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_inputBrowseButton_clicked() {
	this->inputFilename = QFileDialog::getOpenFileName(this, "Open input file", "D:\\");
	ui->inputLineEdit->setText(this->inputFilename);

	this->updateCommand();
}

void MainWindow::on_OutputBrowseButton_clicked() {
	this->outputFilename = QFileDialog::getSaveFileName(this, "Select output file", "D:\\", "WebM files (*.webm)");
	ui->outputLineEdit->setText(this->outputFilename);

	this->updateCommand();
}

void MainWindow::on_convertButton_clicked() {
	this->transcodingProcess->setProcessChannelMode(QProcess::MergedChannels);
	this->transcodingProcess->start(ffmpegPath, this->arguments);
}

void MainWindow::on_videoCodecSelect_currentTextChanged(const QString &arg1) {
	if(arg1 == "VP8") {
		ui->bitrateLabel->setText("Maximum average bitrate:");
		ui->tilecolumnsSpinBox->setEnabled(0);
		ui->frameparallelCheckBox->setEnabled(0);
	} else if(arg1 == "VP9") {
		ui->bitrateLabel->setText("Target bitrate (0 for no target):");
		ui->tilecolumnsSpinBox->setEnabled(1);
		ui->frameparallelCheckBox->setEnabled(1);
	}

	this->updateCommand();
}

void MainWindow::on_inputLineEdit_textChanged(const QString &arg1) {
	this->inputFilename = arg1;
	this->updateCommand();
}

void MainWindow::on_outputLineEdit_textChanged(const QString &arg1) {
	this->outputFilename = arg1;
	this->updateCommand();
}

void MainWindow::on_cancelButton_clicked() {
	this->transcodingProcess->close();
}

void MainWindow::processStarted() {
	ui->convertButton->setEnabled(0);
	ui->cancelButton->setEnabled(1);
}

void MainWindow::processFinished() {
	ui->convertButton->setEnabled(1);
	ui->cancelButton->setEnabled(0);
}

void MainWindow::readyReadStandardOutput() {
	ui->outputBox->append(this->transcodingProcess->readAllStandardOutput());
}

void MainWindow::updateCommand() {
	this->arguments.clear();

	// Gather ffmpeg arguments
	this->arguments << "-i" << this->inputFilename;

	if(ui->videoCodecSelect->currentText() == "VP8") {
		this->arguments << "-c:v" << "libvpx";
	} else if(ui->videoCodecSelect->currentText() == "VP9") {
		this->arguments << "-c:v" << "libvpx-vp9";
		this->arguments << "-tile-columns" << ui->tilecolumnsSpinBox->text();
		if(ui->frameparallelCheckBox->isChecked()) this->arguments << "-frame-parallel" << "1";
	}

	this->arguments << "-crf" << QString::number(ui->CRFSlider->value());
	this->arguments << "-b:v" << ui->bitrateEdit->text();
	this->arguments << "-speed" << QString::number(ui->speedSlider->value());
	this->arguments << "-threads" << QString::number(QThread::idealThreadCount());
	this->arguments << "-pix_fmt" << ui->ColourspaceComboBox->currentText();

	if(ui->audioCheckBox->isChecked()) {
		if(ui->audioCodecSelect->currentText() == "Vorbis") {
			this->arguments << "-c:a" << "libvorbis";
		} else if(ui->audioCodecSelect->currentText() == "Opus") {
			this->arguments << "-c:a" << "libopus";
		}
	}

	if(this->outputFilename == "") {
		this->outputFilename = "output.webm";
	}
	this->arguments << this->outputFilename;

	// Update preview of command
	ui->commandBox->setPlainText(this->arguments.join(QString(" ")));
}
