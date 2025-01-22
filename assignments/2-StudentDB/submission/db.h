#ifndef __DB_H__
    #define __DB_H__

// Basic student database record.  Note:
//  1. id must be > 0.  A student id==0 means the record has been deleted
//  2. gpa is an int, should be between 0<=gpa<=500, real gpa is gpa/100.0 this
//     simplifies dealing with floating point types
//  3. Notice that the student struct was engineered to have a size of
//     64 bytes.  There are reasons for using such a number
typedef struct student{
    int id;
    char fname[24];
    char lname[32];
    int gpa; 
} student_t;

//Define limits for sudent ids and allowable GPA ranges.  Note GPA values will
//be stored as integers but printed as floats.  For example a GPA of 450 is really
//that value divided by 100.0 or 4.50.
#define MIN_STD_ID      1
#define MAX_STD_ID      100000
#define MIN_STD_GPA     0
#define MAX_STD_GPA     500

//some useful constants you should consider using versus hard coding
//in your program. 
static const student_t EMPTY_STUDENT_RECORD = {0};
static const int STUDENT_RECORD_SIZE  = sizeof(struct student);
static const int DELETED_STUDENT_ID = 0;


#define DB_FILE     "student.db"            //name of database file
#define TMP_DB_FILE ".tmp_student.db"       //for extra credit

#endif