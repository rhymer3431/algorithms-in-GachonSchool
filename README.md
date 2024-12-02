
---

# Algorithm Class Term Project: Finding the Shortest Path in a Maze

## Overview
This project involves implementing Dijkstra's algorithm to find the shortest path in a 2D maze. Python is used for data preparation, while C is used to process the maze data, calculate the shortest path, and visualize the results.

---

## Team Members

- 김진하 (202135751)  
- 안재현 (202135795)  
- 임규민 (202135822)  
- 강선구 (202334412)

  
---

## Key Features
1. **Maze Data Creation and Loading**  
   - Uses the `img2txt.py` script to convert an image into numerical data and save it as `map.dat`.  
   - The maze data consists of walls (0), paths (1), a starting point (2), and an endpoint (3).

2. **Dijkstra's Algorithm Implementation**  
   - Implements the shortest path calculation using C, with nodes and a queue structure.  
   - Converts 2D maze coordinates into 1D nodes for efficient traversal.

3. **Result Storage and Visualization**  
   - Saves the shortest path results in `result.dat`.  
   - Uses `txt2img.py` to convert the results into an image (`result.png`) for visual confirmation.

---

## Technologies and Files
- **Programming Languages:** C, Python  
- **File List:**  
   - `maze.c`: Contains the core logic for maze traversal.  
   - `img2txt.py`: Converts images to data files.  
   - `txt2img.py`: Converts data files to images.  
   - `map.dat`: Input data file.  
   - `result.dat`: Result data file.  
   - `result.png`: Final output image.

---

## Usage Instructions
### 1. Environment Setup
- Requires a C compiler and Python installation.
- Python library: `Pillow`

### 2. Execution Steps
1. **Generate Maze Data**
   ```bash
   python img2txt.py
   ```
2. **Run C Code**
   ```bash
   gcc maze.c -o maze
   ./maze
   ```
3. **Visualize Results**
   ```bash
   python txt2img.py
   ```

---

## Results
- The shortest path is displayed as a blue line from the starting point (red) to the endpoint (green).  
- Sample output:  
![ClipboardImage_2024-11-17_205249](https://github.com/user-attachments/assets/aee90560-1eb1-4a8c-b496-d6e4a28bd273)

---

## Additional Information

   - Time Complexity: \(O(N \log N + E)\)

---

## References
- [Dijkstra's Algorithm - Wikipedia](https://en.wikipedia.org/wiki/Dijkstra's_algorithm)  
- [Python Pillow Documentation](https://pillow.readthedocs.io/)

---
