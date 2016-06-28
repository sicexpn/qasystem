#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb
import time


def getConn():
    return MySQLdb.connect("123.57.223.22", "root", "12345", "qa_db")


def insert(question, answer):
    db = getConn();
    sql = """
    insert into search_log (question_test,answer_text,count,cts)
    VALUES (
    """ + question + "," + answer + "," + str(1) + "," + time.strftime("%Y-%m-%d %H:%M:%S", time.localtime()) + ")"

    cursor = db.cursor
    try:
        cursor.execute(sql)
        db.commit()
    except:
        db.rollback()
    db.close()


def insertTest():
    insert("question1", "answer1")

if __name__ == '__main__':
    insertTest()
