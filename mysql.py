#!/usr/bin/python
# -*- coding: UTF-8 -*-

import MySQLdb

db = MySQLdb.connect("123.57.223.22", "root", "12345", "qa_db")
print db
