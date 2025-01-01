#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <unordered_map>
#include <string>
#include <utility>
#include <unordered_map>
#include <fstream>
#include "Compress_Decompress_XML.h"
#include "XML_To_JSON.h"
#include "Validate_XML.h"
#include "Formatting_XML.h"

using namespace std;

std::unordered_map<string, string> dict;
std::string compressed_str;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Browse_Button_clicked()
{
    // Open file dialog to select XML file
    QString xml_file = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(xml_file);

    // Check if file can be opened
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "File could not be opened.");
        return;
    }

    // Read file content and display in plainTextEdit
    QTextStream in(&file);
    QString text = in.readAll();
    ui->lineEdit->setText(xml_file);         // Display the file path
    ui->plainTextEdit->setPlainText(text);  // Display the file content
    file.close();
}

void MainWindow::on_Save_Button_clicked()
{
    QString filter = "All Files (*.*) ;; Text File (*.txt) ;; XML File (*.xml)";
    QString file_path = QFileDialog::getSaveFileName(this, "Save a file", "C://", filter);
    QFile compressed_file(file_path);  // Initialize QFile with the file path
    if (!compressed_file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Error", "File not open");
        return;
    }
    QTextStream out(&compressed_file);
    QString compressed_text = ui->plainTextEdit_2->toPlainText();
    out << compressed_text;
    ui->plainTextEdit_2->clear();
    compressed_file.flush();
    compressed_file.close();
}

void MainWindow::on_Compress_Button_clicked()
{
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    // Open the file and read its content
    QFile file(inputFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open the file for reading!");
        return;
    }

    QString fileContent = file.readAll(); // Read the file content as QString
    file.close();

    // Convert QString content to std::string
    std::string xmlContent = fileContent.toStdString();

    // Pass the XML content to the ApplyBPE function
    compressed_str = ApplyBPE(xmlContent, dict);

    // Convert the compressed result back to QString
    QString compressed_qstr = QString::fromStdString(compressed_str);

    // Display the compressed content in QPlainTextEdit
    ui->plainTextEdit_2->setPlainText(compressed_qstr);

    QMessageBox::information(this, "Success", "XML file has been compressed and displayed successfully!");
}


void MainWindow::on_Decompress_Button_clicked() {
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

   std::string inputPath = inputFile.toStdString();

    // Call the decompress function
    std::string decompressed_string = Decompress(compressed_str,dict);

    QString decompressed_qstr = QString::fromStdString(decompressed_string);
    ui->plainTextEdit_2->setPlainText(decompressed_qstr);

    QMessageBox::information(this, "Success", "XML file has been decompressed and displayed successfully!");

}

void MainWindow::on_Prettify_Button_clicked()
{
    // Step 1: Select the input XML file

    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    // Step 2: Define the temporary output file for prettified XML
    QString outputFile = "temp_prettified.xml"; // Temporary file for prettified output
    std::string inputPath = inputFile.toStdString();
    std::string outputPath = outputFile.toStdString();

    // Step 3: Call the prettifyXML function
    prettifyXML(inputPath, outputPath);

    // Step 4: Read the prettified XML from the output file
    QFile file(QString::fromStdString(outputPath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open prettified XML file!");
        return;
    }

    QTextStream in(&file);
    QString prettifiedContent = in.readAll();
    file.close();

    // Step 5: Display the prettified XML content in QPlainTextEdit
    ui->plainTextEdit_2->setPlainText(prettifiedContent);

    // Step 6: Optionally remove the temporary output file
    QFile::remove(outputFile);

    // Step 7: Notify the user of success
    QMessageBox::information(this, "Success", "XML file has been prettified and displayed successfully!");
}



void MainWindow::on_Minify_Button_clicked()
{

        QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
        if (inputFile.isEmpty()) {
            QMessageBox::warning(this, "Error", "No input file selected!");
            return;
        }

        QString outputFile = "temp_minified.xml"; // Temporary file for minified output
        std::string inputPath = inputFile.toStdString();
        std::string outputPath = outputFile.toStdString();

        // Call the minifyXML function
        minifyXML(inputPath, outputPath);

        // Read the minified XML from the output file
        QFile file(QString::fromStdString(outputPath));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Failed to open minified XML file!");
            return;
        }

        QTextStream in(&file);
        QString minifiedContent = in.readAll();
        file.close();

        // Display the minified XML content in QPlainTextEdit
        ui->plainTextEdit_2->setPlainText(minifiedContent);

        // Optionally remove the temporary output file
        QFile::remove(outputFile);

        QMessageBox::information(this, "Success", "XML file has been minified and displayed successfully!");


}


void MainWindow::on_Validate_Button_clicked()
{
    // Step 1: Select the input XML file
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QFile file(inputFile);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    QString text = in.readAll();

    // Step 3: Call the validateXML function

    bool validation_flag = is_valid(text);
    if (!validation_flag) {
        QMessageBox::warning(this, "Warning", "The XML file is not valid, the textbox shows the validated string!");
    }

    QString validated_string = corrected_xml(text); // Replace with your actual validation function
    // Step 4: Display the success message in QPlainTextEdit
    ui->plainTextEdit_2->setPlainText(validated_string);

    // Step 5: Notify the user of success
    QMessageBox::information(this, "Validation Success", "The XML file has been validated successfully!");
}


void MainWindow::on_ConvertToJSON_Button_clicked()
{
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QFile file(inputFile);

    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    QString text = in.readAll();
    std::string xml_Str = text.toStdString();
    std::string json_Str;
    convertXMLToJSON(xml_Str,json_Str);
    QString json_qStr = QString::fromStdString(json_Str);

    ui->plainTextEdit_2->setPlainText(json_qStr);

    // Step 5: Notify the user of success
    QMessageBox::information(this, "Conversion Success", "The XML file has been converted to JSON file successfully!");

}



/*
void MainWindow::on_Compress_Button_clicked()
{
    QString filter = "All Files (*.*) ;; Text File (*.txt) ;; XML File (*.xml)";
    QString file_name = QFileDialog::getOpenFileName(this,"open a file","C://",filter);
    if(file_name.isEmpty())
    {
        return;
    }
    QFile file(file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    QString text = QString::fromUtf8(file.readAll());

    file.close();
    std::string stdStr = text.toStdString();
    compressed_str = ApplyBPE(stdStr, dict);

    qDebug() << "Dictionary after compression:";
    for (const auto& pair : dict) {
        qDebug() << QString::fromStdString(pair.first) << " -> " << QString::fromStdString(pair.second);
    }


    if(compressed_str.empty())
    {
        QMessageBox::warning(this,"Error","Compression failed.");
        return;
    }

    QString qStr = QString::fromStdString(compressed_str);
    ui->plainTextEdit_2->setPlainText(qStr);

}

void MainWindow::on_Decompress_Button_clicked() {
    if (dict.empty()) {
        QMessageBox::warning(this, "Error", "Dictionary is empty! Perform compression first.");
        return;
    }

    QString inputFile = QFileDialog::getOpenFileName(this, "Select Compressed XML File", "C://", "Text Files (*.txt);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QFile compressedFile(inputFile);
    if (!compressedFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Failed to open compressed XML file!");
        return;
    }

    QTextStream inCompressed(&compressedFile);
    QString compressedContent = inCompressed.readAll();
    compressedFile.close();
    //std::string compressed_str = compressedContent.toStdString();
    std::string decompressedContent = Decompress(compressed_str, dict);


    qDebug() << "Dictionary after decompression:";
    for (const auto& pair : dict) {
        qDebug() << QString::fromStdString(pair.first) << " -> " << QString::fromStdString(pair.second);
    }

    ui->plainTextEdit_2->setPlainText(QString::fromStdString(decompressedContent));

    QMessageBox::information(this, "Success", "The XML file has been decompressed successfully!");
}
*/

