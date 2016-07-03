#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb
import time


def getConn():
    return MySQLdb.connect("123.57.223.22", "root", "12345", "qa_db")


def insert(question, answer):
    db = getConn();
    ctime = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
    sql = "insert into search_log(question_text, answer_text, cts, uts) \
    VALUES ('%s','%s','%d','%s','%s')" % \
          (question, answer, ctime, ctime)

    print "sql is " + sql
    cursor = db.cursor()
    try:
        cursor.execute(sql)
        db.commit()
        print "insert ok"
    except:
        db.rollback()
        print "insert failed"
    db.close()


def select(question):
    db = getConn();
    sql = "select id,question_text,answer_text,cts,uts from search_log where question = '" + question + "'";
    cursor = db.cursor()
    results = cursor.execute(sql)
    for row in results:
        id = row[0]
        question_text = row[1]
        answer_text = row[2]
        cts = row[3]
        uts = row[4]

def insertTest():
    insert("question1", "answer1")


if __name__ == '__main__':
    insertTest()
