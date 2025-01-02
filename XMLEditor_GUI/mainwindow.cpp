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
#include "graph.h"
#include "graph_visualization.h"
#include <QInputDialog>
#include <map>
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

/*
void MainWindow::on_Compress_Button_clicked()
{
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "C://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QFile file(inputFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open the file for reading!");
        return;
    }

    QString fileContent = file.readAll(); // Read the file content as QString
    file.close();

    // Convert QString content to std::string
    std::string xmlContent = fileContent.toStdString();

    QString outputFile = QFileDialog::getSaveFileName(this, "Save Compressed File", "compressed_output.txt", "Text Files (*.txt);;All Files (*.*)");
    if (outputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No output file selected!");
        return;
    }

    try {
        // Compress and save to file
        writeCompressedFile(xmlContent, outputFile.toStdString());
        QMessageBox::information(this, "Success", "XML file has been compressed and saved successfully!");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Compression failed: %1").arg(e.what()));
    }
}

void MainWindow::on_Decompress_Button_clicked()
{
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Compressed File", "C://", "Text Files (*.txt);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    try {
        // Read compressed content and decompress it
        std::string decompressedString = readCompressedFile(inputFile.toStdString());

        // Display decompressed content in the text editor
        QString decompressedContent = QString::fromStdString(decompressedString);
        ui->plainTextEdit_2->setPlainText(decompressedContent);

        QMessageBox::information(this, "Success", "File has been decompressed and displayed successfully!");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Decompression failed: %1").arg(e.what()));
    }
}*/




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
    json_Str = convertXMLToJSON(xml_Str);
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


void MainWindow::on_MostInfluential_clicked()
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
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* node = parseXML(xml_content,pos);
    map<int, vector<int>>graph;
    map<int,std::string> idNameMap;
    buildGraph(node,graph);
    buildIdNameMap(node,idNameMap);
    vector<pair<int, std::string>> most_influential;
    most_influential = findMostInfluentialUsers(graph,idNameMap);
    QString result = "Most Influential User:\n*******************\n";
    for (const auto& [id, name] : most_influential) {
        result += QString("ID: %1, Name: %2\n").arg(id).arg(QString::fromStdString(name));
    }

    // Display the result in plainTextEdit
    ui->plainTextEdit_2->setPlainText(result);

    QMessageBox::information(this, "Success", "Most influential users have been displayed!");

}


void MainWindow::on_MostActive_clicked()
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
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* node = parseXML(xml_content,pos);
    map<int, vector<int>>graph;
    map<int,std::string> idNameMap;
    map<int, vector<int>> followingMap;
    buildGraph(node,graph);
    buildIdNameMap(node,idNameMap);
    buildFollowingMap(graph,followingMap);
    vector<pair<int, string>> most_active;
    most_active = findMostActiveUsers(graph, idNameMap, followingMap);
    QString result = "Most Active User:\n*******************\n";
    for (const auto& [id, name] : most_active) {
        result += QString("ID: %1, Name: %2\n").arg(id).arg(QString::fromStdString(name));
    }

    // Display the result in plainTextEdit
    ui->plainTextEdit_2->setPlainText(result);

    QMessageBox::information(this, "Success", "Most active users have been displayed!");
}


void MainWindow::on_MutualFollowers_clicked()
{
    // Open file dialog to select XML file
    QString xml_file = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(xml_file);

    // Check if file can be opened
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "File could not be opened.");
        return;
    }

    // Read file content
    QTextStream in(&file);
    QString text = in.readAll();
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* node = parseXML(xml_content, pos);

    // Build necessary data structures
    map<int, vector<int>> graph;
    map<int, std::string> idNameMap;
    map<int, vector<int>> followingMap;
    buildGraph(node, graph);
    buildIdNameMap(node, idNameMap);
    buildFollowingMap(graph, followingMap);

    // Prompt user for IDs
    bool ok;
    QString input = QInputDialog::getText(this, "Enter User IDs",
                                          "Enter comma-separated User IDs:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || input.isEmpty()) {
        QMessageBox::information(this, "Info", "No IDs provided.");
        return;
    }

    // Parse input into a vector of integers
    std::vector<int> users;
    for (const auto& id_str : input.split(",")) {
        bool conversionOk;
        int id = id_str.trimmed().toInt(&conversionOk);
        if (conversionOk) {
            users.push_back(id);
        }
    }

    // Find mutual followers
    std::vector<int> mutual_followers = findMutualFollowers(graph, users, users.size());

    // Prepare result string
    QString result = "Mutual Followers:\n";
    for (int id : mutual_followers) {
        result += QString::number(id) + " (" + QString::fromStdString(idNameMap[id]) + ")\n";
    }

    // Display result
    if (mutual_followers.empty()) {
        result = "No mutual followers found.";
    }
    ui->plainTextEdit_2->setPlainText(result);
}



void MainWindow::on_Suggest_clicked()
{
    // Open file dialog to select XML file
    QString xml_file = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(xml_file);

    // Check if file can be opened
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "File could not be opened.");
        return;
    }

    // Read file content
    QTextStream in(&file);
    QString text = in.readAll();
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* node = parseXML(xml_content, pos);

    // Prompt user for IDs
    bool ok;
    QString input = QInputDialog::getText(this, "Enter User ID",
                                          "Enter User ID:",
                                          QLineEdit::Normal, "", &ok);
    int userId = input.toInt();
    if (!ok || input.isEmpty()) {
        QMessageBox::information(this, "Info", "No IDs provided.");
        return;
    }

    map<int, vector<int>> graph;
    map<int, std::string> idNameMap;
    vector<int> suggested_users;
    buildGraph(node, graph);
    buildIdNameMap(node, idNameMap);

    suggested_users = suggestUsersToFollowForId(graph,userId,idNameMap);

    QString result = "Suggested Users:\n";
    for(int id:suggested_users)
        result += QString::number(id) + " (" + QString::fromStdString(idNameMap[id]) + ")\n";


    // Display result
    if (suggested_users.empty()) {
        result = "No followers to be suggested.";
    }
    ui->plainTextEdit_2->setPlainText(result);
}


void MainWindow::on_SearchByWord_clicked()
{
    // Open file dialog to select XML file
    QString xml_file = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(xml_file);

    // Check if file can be opened
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "File could not be opened.");
        return;
    }

    // Read file content
    QTextStream in(&file);
    QString text = in.readAll();
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* root = parseXML(xml_content, pos);
    vector<string> bodies;

    if (!root) {
        QMessageBox::warning(this, "Error", "Failed to parse XML file.");
        return;
    }

    // Prompt user for the search word
    bool ok;
    QString word = QInputDialog::getText(this, "Search a Word",
                                         "Enter a word to search for:",
                                         QLineEdit::Normal, "", &ok);
    if (!ok || word.isEmpty()) {
        QMessageBox::information(this, "Info", "No word provided.");
        return;
    }

    // Search for bodies containing the word
    bodies = searchByWord(root, word.toStdString());

    // Construct the result string
    QString result = "Posts mentioning the word \"" + word + "\":\n\n";
    if (bodies.empty()) {
        result += "No posts found containing the word \"" + word + "\".";
    } else {
        for (size_t i = 0; i < bodies.size(); ++i) {
            result += QString("Post %1: %2\n").arg(i + 1).arg(QString::fromStdString(bodies[i]));
        }
    }

    // Display the result in the plain text edit
    ui->plainTextEdit_2->setPlainText(result);
}



void MainWindow::on_SearchByTopic_clicked()
{
    QString inputFile = QFileDialog::getOpenFileName(this, "Select Input XML File", "D://", "XML Files (*.xml);;All Files (*.*)");
    if (inputFile.isEmpty()) {
        QMessageBox::warning(this, "Error", "No input file selected!");
        return;
    }

    QString outputFile = "searchByTopic_minified.xml"; // Temporary file for minified output
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

    std::string xml_content = minifiedContent.toStdString();
    size_t pos = 0;
    TreeNode* root = parseXML(xml_content, pos);

    // Check if root is valid
    if (!root) {
        QMessageBox::warning(this, "Error", "Failed to parse XML file.");
        return;
    }

    // Prompt user for the topic
    bool ok;
    QString topic = QInputDialog::getText(this, "Search a Topic",
                                          "Enter a topic to search for:",
                                          QLineEdit::Normal, "", &ok);
    if (!ok || topic.isEmpty()) {
        QMessageBox::information(this, "Info", "No topic provided.");
        return;
    }

    // Search for bodies containing the topic
    vector<string> bodies = searchByTopic(root, topic.toStdString());

    // Construct the result string
    QString result = "Posts containing the topic \"" + topic + "\":\n\n";
    if (bodies.empty()) {
        result += "No posts found containing the topic \"" + topic + "\".";
    } else {
        for (size_t i = 0; i < bodies.size(); ++i) {
            result += QString("Post %1: %2\n").arg(i + 1).arg(QString::fromStdString(bodies[i]));
        }
    }

    // Display the result in the plain text edit
    ui->plainTextEdit_2->setPlainText(result);
}


void MainWindow::on_DrawGraph_clicked()
{
    // Open file dialog to select XML file
    QString xml_file = QFileDialog::getOpenFileName(this, "Open a file", "C://");
    QFile file(xml_file);

    // Check if file can be opened
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "File could not be opened.");
        return;
    }

    // Read file content
    QTextStream in(&file);
    QString text = in.readAll();
    std::string xml_content = text.toStdString();
    size_t pos = 0;
    TreeNode* node = parseXML(xml_content, pos);
    map<int, vector<int>> graph;
    buildGraph(node, graph);
    visualize_graph(graph);
}

