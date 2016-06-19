f = open('data_train.txt','r')
lines = f.readlines()
f.close()


ques = open('questions.txt','w')
ans = open('answer.txt','w')
words2id = open('wordsdict.txt','w')
entity2id = open('entitydict.txt','w')

qu = []
question_list = []
answer_list = []
for line in lines:
	line = line.strip()
	question,answer = line.split('\t')
	qu.append(question.strip())
	q_list = question.split(' ')
	for i in q_list:
		question_list.append(i.strip())
	answer_list.append(answer.strip())
 
q = set(question_list)
a = set(answer_list)


for i in qu:
	print >>ques, i.strip()
idx2= 0
for i in answer_list:
	print >>ans, i.strip()
	print >>entity2id, i.strip()+' '+str(idx2)
	idx2+=1

idx=0

for i in q:
	print >>words2id, i.strip()+' '+str(idx)
        idx+=1


