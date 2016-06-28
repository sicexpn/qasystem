#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb
import time


def getConn():
    return MySQLdb.connect("123.57.223.22", "root", "12345", "qa_db")


def insert(question, answer):
    db = getConn();
    ctime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    sql = "insert into search_log(question_text, answer_text, count, cts, uts) \
    VALUES ('%s','%s','%d','%s','%s')" % \
    (question ,answer ,1,ctime ,ctime)

    print "sql is " + sql
    cursor = db.cursor
    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
        print "insert failed"
    db.close()


def insertTest():
    insert("question1", "answer1")


if __name__ == '__main__':
    insertTest()
