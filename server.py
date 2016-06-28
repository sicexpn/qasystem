#!/usr/bin/python
# -*- coding: utf-8 -*-
from flask import Flask
from flask import render_template,request
import os
import sys

reload(sys)  
sys.setdefaultencoding('utf8') 
app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')
@app.route('/submit',methods=['POST'])
def submitQuestion():
    if request.method=='POST':
	question=request.form['question']
	
	q_file=open('./qa/questions_for_test.txt','w')
	print >>q_file,question
	q_file.close()
	out=os.popen('./getAnswer.sh')
	answers=out.read()
	print "answer.."
	print answers
	answerlist=answers.strip().split('\n')
	return render_template('index.html',question=question,answers=answerlist)
if __name__ == '__main__':
    app.run(
	host="0.0.0.0",
	port=8888,
	debug=True)
