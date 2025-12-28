#!/usr/bin/env python3
"""
Test suite for Simple Database (sdbsc) assignment
Converted from BATS to pytest
"""

import subprocess
import os
import pytest


# Setup fixture that runs once before all tests
@pytest.fixture(scope="session", autouse=True)
def setup_test_environment():
    """Delete student.db file if it exists before running tests"""
    if os.path.exists("student.db"):
        os.remove("student.db")
    yield
    # Cleanup after all tests (optional)
    # if os.path.exists("student.db"):
    #     os.remove("student.db")


def run_sdbsc(*args):
    """
    Helper function to run sdbsc with arguments
    Returns (returncode, stdout, stderr)
    """
    cmd = ["./sdbsc"] + list(args)
    result = subprocess.run(
        cmd,
        capture_output=True,
        text=True
    )
    return result.returncode, result.stdout, result.stderr


def normalize_whitespace(text):
    """Normalize multiple spaces to single space and strip"""
    return ' '.join(text.split())


class TestDatabaseBasics:
    """Test basic database operations"""
    
    def test_01_database_empty_at_start(self):
        """Check if database is empty to start"""
        returncode, stdout, stderr = run_sdbsc("-p")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        assert stdout.strip() == "Database contains no student records."
    
    def test_02_add_student_1(self):
        """Add student 1 to database"""
        returncode, stdout, stderr = run_sdbsc("-a", "1", "john", "doe", "345")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 1 added to database."
    
    def test_03_add_more_students(self):
        """Add multiple students to database"""
        # Add student 3
        returncode, stdout, stderr = run_sdbsc("-a", "3", "jane", "doe", "390")
        assert returncode == 0, f"Expected return code 0, got {returncode}\nOutput: {stdout}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 3 added to database.", f"Failed Output: {stdout}"
        
        # Add student 63
        returncode, stdout, stderr = run_sdbsc("-a", "63", "jim", "doe", "285")
        assert returncode == 0, f"Expected return code 0, got {returncode}\nOutput: {stdout}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 63 added to database.", f"Failed Output: {stdout}"
        
        # Add student 64
        returncode, stdout, stderr = run_sdbsc("-a", "64", "janet", "doe", "310")
        assert returncode == 0, f"Expected return code 0, got {returncode}\nOutput: {stdout}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 64 added to database.", f"Failed Output: {stdout}"
        
        # Add student 99999
        returncode, stdout, stderr = run_sdbsc("-a", "99999", "big", "dude", "205")
        assert returncode == 0, f"Expected return code 0, got {returncode}\nOutput: {stdout}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 99999 added to database.", f"Failed Output: {stdout}"
    
    def test_04_check_student_count(self):
        """Check student count is 5"""
        returncode, stdout, stderr = run_sdbsc("-c")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Database contains 5 student record(s).", f"Failed Output: {stdout}"
    
    def test_05_add_duplicate_student_fails(self):
        """Make sure adding duplicate student fails"""
        returncode, stdout, stderr = run_sdbsc("-a", "63", "dup", "student", "300")
        assert returncode == 1, f"Expected return code 1, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Cant add student with ID=63, already exists in db.", f"Failed Output: {stdout}"
    
    def test_06_check_file_size(self):
        """Make sure the file size is correct"""
        file_size = os.path.getsize("student.db")
        assert file_size == 6400000, f"Expected file size 6400000, got {file_size}"


class TestDatabaseSearch:
    """Test database search operations"""
    
    def test_07_find_student_3(self):
        """Find student 3 in database"""
        returncode, stdout, stderr = run_sdbsc("-f", "3")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        
        lines = stdout.strip().split('\n')
        # Second line should be the student record (first line is header)
        normalized_output = normalize_whitespace(lines[1])
        expected_output = "3 jane doe 3.90"
        
        assert normalized_output == expected_output, \
            f"Failed Output: {normalized_output}\nExpected: {expected_output}"
    
    def test_08_find_nonexistent_student(self):
        """Try looking up non-existent student"""
        returncode, stdout, stderr = run_sdbsc("-f", "4")
        assert returncode == 1, f"Expected return code 1, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 4 was not found in database.", f"Failed Output: {stdout}"


class TestDatabaseDelete:
    """Test database delete operations"""
    
    def test_09_delete_student_64(self):
        """Delete student 64 from database"""
        returncode, stdout, stderr = run_sdbsc("-d", "64")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 64 was deleted from database.", f"Failed Output: {stdout}"
    
    def test_10_delete_nonexistent_student(self):
        """Try deleting non-existent student"""
        returncode, stdout, stderr = run_sdbsc("-d", "65")
        assert returncode == 1, f"Expected return code 1, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 65 was not found in database.", f"Failed Output: {stdout}"
    
    def test_11_check_student_count_after_delete(self):
        """Check student count is 4 after deletion"""
        returncode, stdout, stderr = run_sdbsc("-c")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Database contains 4 student record(s).", f"Failed Output: {stdout}"


class TestDatabasePrint:
    """Test database print operations"""
    
    def test_12_print_student_records(self):
        """Print all student records"""
        returncode, stdout, stderr = run_sdbsc("-p")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        
        # Normalize the output
        normalized_output = normalize_whitespace(stdout.strip())
        
        # Expected output (normalized)
        expected_output = "ID FIRST_NAME LAST_NAME GPA 1 john doe 3.45 3 jane doe 3.90 63 jim doe 2.85 99999 big dude 2.05"
        
        assert normalized_output == expected_output, \
            f"Failed Output: {normalized_output}\nExpected: {expected_output}"


class TestDatabaseCompress:
    """Test database compression (extra credit)"""
    
    def test_13_compress_db_try_1(self):
        """Compress database - first attempt"""
        returncode, stdout, stderr = run_sdbsc("-x")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Database successfully compressed!", f"Failed Output: {stdout}"
    
    def test_14_delete_student_99999(self):
        """Delete student 99999 from database"""
        returncode, stdout, stderr = run_sdbsc("-d", "99999")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Student 99999 was deleted from database.", f"Failed Output: {stdout}"
    
    def test_15_compress_db_try_2(self):
        """Compress database - second attempt"""
        returncode, stdout, stderr = run_sdbsc("-x")
        assert returncode == 0, f"Expected return code 0, got {returncode}"
        lines = stdout.strip().split('\n')
        assert lines[0] == "Database successfully compressed!", f"Failed Output: {stdout}"


if __name__ == "__main__":
    # Run pytest when script is executed directly
    pytest.main([__file__, "-v"])