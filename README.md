# XML Editor Project

## CSE331: Data Structures and Algorithms

### Functionalities
#### Basic Features
1. **XML Consistency Check** 🛠️
   - Detect inconsistencies like missing or mismatched tags.
   - Visualize errors and provide automated fixes.

2. **Formatting (Prettifying)** ✨
   - Format XML files with proper indentation for readability.

3. **Conversion to JSON** 🔄
   - Convert XML files into JSON format for interoperability with JavaScript-based tools.

4. **Minification** 📉
   - Remove unnecessary spaces and newlines to reduce file size.

5. **Compression and Decompression** 📦
   - Implement a data compression algorithm to reduce file size efficiently.
   - Decompress files to restore their original format.

#### Advanced Features
1. **Graph Representation of XML Data** 📊
   - Represent users in a social network as a graph.
   - Visualize user relationships (followers, posts) and save the graph as an image.

2. **Network Analysis** 📈
   - Identify the most influential and active users.
   - Suggest users to follow based on mutual connections.

3. **Post Search** 🔍
   - Search for posts by specific words or topics.

4. **Graph Visualization** 🖼️
   - Use third-party libraries or tools to display user relationships graphically.

### Command-Line Operations
| **Operation**         | **Command**                                             | **Description**                                                                 |
|-----------------------|-------------------------------------------------------|-------------------------------------------------------------------------------|
| Check Consistency     | `xml_editor verify -i input_file.xml -f -o output.xml` | Validate XML and optionally fix errors, saving the corrected file.            |
| Format XML            | `xml_editor format -i input_file.xml -o output.xml`    | Format the XML file and save the result.                                      |
| Convert to JSON       | `xml_editor json -i input_file.xml -o output.json`     | Convert XML to JSON and save the output.                                      |
| Minify XML            | `xml_editor mini -i input_file.xml -o output.xml`      | Minify the XML file to reduce size.                                           |
| Compress XML          | `xml_editor compress -i input_file.xml -o output.comp` | Compress the XML file using a custom algorithm.                               |
| Decompress XML        | `xml_editor decompress -i input.comp -o output.xml`    | Decompress a compressed XML file.                                             |
| Network Analysis      | `xml_editor most_active -i input_file.xml`             | Identify the most active user.                                                |
| Post Search           | `xml_editor search -w word -i input_file.xml`          | Search posts containing a specific word.                                      |

### GUI Features
- 🗂️ Provide an input file via a browse button or directly in the application.
- 🎛️ Perform operations (verify, format, convert, etc.) via buttons.
- 📄 Display outputs in a read-only text field with an option to save results.

### Demonstration Links
- **Command Line Mode**: [View Demo](https://drive.google.com/file/d/1n72GIMFJqBsHLI7SXZCBCxUnTMLjd1BX/view?usp=drive_link)
- **GUI Mode**: [View Demo](https://drive.google.com/file/d/1KoW-f0Oz_Hs77si9CYtLAzwSz262S4Xt/view?usp=drive_link)
- **Project Report**: [View Report](https://drive.google.com/file/d/1vMgflwulpS0mCYdI4IGalR9BFQ20vMRb/view?usp=drive_link)


### Why This Project?
🚀 This project offers practical experience in:
- Parsing and handling XML and JSON files.
- Designing user-friendly graphical interfaces.
- Applying optimal data structures to solve real-world problems.


