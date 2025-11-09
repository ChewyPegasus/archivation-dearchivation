#!/bin/bash

# --- Configuration ---
PROJECT_NAME="arch_dearch"
SRC_DIR="src"
ARCHIVER_DIR="$SRC_DIR/archiver"
BUILD_DIR="build"

# --- Script Logic ---
echo "🔧 Building '$PROJECT_NAME' project..."
echo "📂 Project structure:"
echo "   $SRC_DIR/"
echo "   ├── main.cpp"
echo "   └── $ARCHIVER_DIR/"
echo "       ├── archiver.h"
echo "       └── archiver.cpp"
echo ""

# Check if essential source files exist
if [ ! -f "$SRC_DIR/main.cpp" ] || [ ! -f "$ARCHIVER_DIR/archiver.h" ]; then
    echo "❌ Error: Source files not found in the expected directory structure."
    echo "   Please ensure your files are organized as shown above."
    exit 1
fi

# Clean previous build directory
if [ -d "$BUILD_DIR" ]; then
    echo "📦 Cleaning previous build..."
    rm -rf "$BUILD_DIR"
fi

# Create and enter build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure with CMake
echo "⚙️  Configuring CMake..."
# The '||' part ensures the script exits if cmake fails
cmake .. || { echo "❌ CMake configuration failed"; exit 1; }

# Determine the number of CPU cores for parallel build
get_cores() {
    if [[ "$OSTYPE" == "darwin"* ]]; then # macOS
        sysctl -n hw.ncpu
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then # Linux
        nproc
    else # Fallback for other systems (like Git Bash on Windows)
        echo 4
    fi
}
CORES=$(get_cores)

# Build the project using cmake's build tool mode
echo "🔨 Building with $CORES cores..."
cmake --build . --parallel "$CORES" || { echo "❌ Build failed"; exit 1; }

echo "✅ Build successful!"
echo "📍 Executable: $(pwd)/$PROJECT_NAME"

# Return to root project directory
cd ..

# Create a sample test file if it doesn't exist
if [ ! -f "input.txt" ]; then
    echo "📝 Creating sample 'input.txt' file..."
    cat > input.txt << EOF
AAAAAABBBBBCCCCCDDDDD
Hello World!!!
1111122222333334444455555
This is a test line with no repeats.
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
EOF
fi

echo ""
echo "🚀 To run the program:"
echo "   ./$BUILD_DIR/$PROJECT_NAME"
echo ""
echo "📝 Files will be:"
echo "   Input:      input.txt"
echo "   Compressed: archived.txt"
echo "   Output:     output.txt"
