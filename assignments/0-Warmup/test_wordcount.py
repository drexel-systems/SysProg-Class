import subprocess
import pytest
import os
from pathlib import Path

# Path to the compiled binary
BINARY = "./wordcount"

@pytest.fixture(scope="session", autouse=True)
def compile_binary():
    """Compile the C program before running tests"""
    result = subprocess.run(["make", "clean"], capture_output=True)
    result = subprocess.run(["make"], capture_output=True, text=True)
    if result.returncode != 0:
        pytest.fail(f"Compilation failed:\n{result.stderr}")
    
    if not os.path.exists(BINARY):
        pytest.fail(f"Binary {BINARY} was not created")
    
    yield
    
    # Cleanup
    subprocess.run(["make", "clean"], capture_output=True)

@pytest.fixture
def sample_file(tmp_path):
    """Create a temporary file with known content"""
    file = tmp_path / "test.txt"
    content = "Hello world\nThis is a test\nWith three lines\n"
    file.write_text(content)
    return file

@pytest.fixture
def multi_files(tmp_path):
    """Create multiple test files"""
    file1 = tmp_path / "file1.txt"
    file2 = tmp_path / "file2.txt"
    
    file1.write_text("First file\nTwo lines\n")
    file2.write_text("Second file\n")
    
    return file1, file2


class TestBasicFunctionality:
    """Test core counting functionality"""
    
    def test_stdin_all_counts(self):
        """Test counting from stdin with all options"""
        input_text = "Hello world\nSecond line\n"
        result = subprocess.run(
            [BINARY],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        # Should show: 2 lines, 4 words, 24 chars
        parts = output.split()
        assert len(parts) == 3
        assert parts[0] == "2"  # lines
        assert parts[1] == "4"  # words
        assert parts[2] == "24" # chars
    
    def test_file_all_counts(self, sample_file):
        """Test counting from a file with all options"""
        result = subprocess.run(
            [BINARY, str(sample_file)],
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 4  # 3 counts + filename
        assert parts[0] == "3"  # lines
        assert parts[1] == "9"  # words (Hello world This is a test With three lines)
        assert parts[3] == str(sample_file)  # filename


class TestOptions:
    """Test individual option flags"""
    
    def test_lines_only(self):
        """Test -l option (lines only)"""
        input_text = "Line 1\nLine 2\nLine 3\n"
        result = subprocess.run(
            [BINARY, "-l"],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 1
        assert parts[0] == "3"
    
    def test_words_only(self):
        """Test -w option (words only)"""
        input_text = "one two three\nfour five\n"
        result = subprocess.run(
            [BINARY, "-w"],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 1
        assert parts[0] == "5"
    
    def test_chars_only(self):
        """Test -c option (characters only)"""
        input_text = "abc\n"
        result = subprocess.run(
            [BINARY, "-c"],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 1
        assert parts[0] == "4"  # 3 chars + newline
    
    def test_multiple_options(self):
        """Test combining -l and -w"""
        input_text = "Hello world\nGoodbye world\n"
        result = subprocess.run(
            [BINARY, "-l", "-w"],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 2
        assert parts[0] == "2"  # lines
        assert parts[1] == "4"  # words


class TestMultipleFiles:
    """Test handling multiple input files"""
    
    def test_two_files(self, multi_files):
        """Test processing two files"""
        file1, file2 = multi_files
        result = subprocess.run(
            [BINARY, str(file1), str(file2)],
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        lines = result.stdout.strip().split('\n')
        assert len(lines) == 3  # file1, file2, total
        
        # Check that last line ends with "total"
        assert lines[-1].endswith(" total")
    
    def test_single_file_no_total(self, sample_file):
        """Test that single file doesn't show total"""
        result = subprocess.run(
            [BINARY, str(sample_file)],
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        lines = result.stdout.strip().split('\n')
        assert len(lines) == 1  # Only one line of output
        # Check that no line ends with "total" (avoid matching "total" in directory paths)
        assert not any(line.endswith(" total") for line in lines)


class TestErrorHandling:
    """Test error cases"""
    
    def test_nonexistent_file(self):
        """Test handling of non-existent file"""
        result = subprocess.run(
            [BINARY, "nonexistent_file.txt"],
            capture_output=True,
            text=True
        )
        
        assert result.returncode != 0
        assert "cannot open" in result.stderr.lower() or "error" in result.stderr.lower()
    
    def test_invalid_option(self):
        """Test handling of invalid option"""
        result = subprocess.run(
            [BINARY, "-x"],
            capture_output=True,
            text=True
        )
        
        assert result.returncode != 0
        assert "unknown" in result.stderr.lower() or "usage" in result.stderr.lower()


class TestEdgeCases:
    """Test edge cases"""
    
    def test_empty_input(self):
        """Test with empty input"""
        result = subprocess.run(
            [BINARY],
            input="",
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert len(parts) == 3
        assert all(p == "0" for p in parts)
    
    def test_no_trailing_newline(self):
        """Test file without trailing newline"""
        input_text = "No newline at end"
        result = subprocess.run(
            [BINARY],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        parts = output.split()
        assert parts[0] == "0"  # No lines (no newline)
        assert parts[1] == "4"  # 4 words
    
    def test_multiple_spaces(self):
        """Test that multiple spaces count as one word separator"""
        input_text = "word1    word2\n"
        result = subprocess.run(
            [BINARY, "-w"],
            input=input_text,
            capture_output=True,
            text=True
        )
        
        assert result.returncode == 0
        output = result.stdout.strip()
        assert output == "2"