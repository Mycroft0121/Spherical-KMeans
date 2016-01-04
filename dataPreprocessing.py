from glob import glob
import os
import re
import collections

def tail(f, n):
	stdin,stdout = os.popen2("tail -n "+ str(n) + " " + f)
	stdin.close()
	lines = stdout.readlines(); stdout.close()
	return ' '.join(lines[-n:])

def newsGroup(newsGroupList):
	
	f1 = open('newsgroups.class', 'w')
	f2 = open('newsgroups.rlabel', 'w')
	
	for item in newsGroupList:
		f1.write(str(item[0]) + ',' + item[1] + '\n')
		f2.write(str(item[0]) + ',' + item[1] + '_' + str(item[2]) + '\n')
	f1.close()
	f2.close() 		 

def bagOfWords(tokenStrings):
	
	dimentions = {}
	bagOfWords = []
	countD = 0

	for token in tokenStrings:
		tokens = token[1].split()
                counter = collections.Counter(tokens)
                for item in counter.keys():
                        bagOfWords.append([token[0], item, counter[item]])
#			print item
                        if item not in dimentions:
                                countD += 1
                                dimentions[item] = countD

	f = open('bag.csv','w')
	for item in bagOfWords:
#		print item[0], item[1], item[2]
	        f.write(str(objectMap[item[0]]) + ',' + str(dimentions[item[1]]) + ',' + str(item[2]) + '\n')
	f.close()
	
	f = open('bag.clabel', 'w')
	for item in dimentions.keys():
        	f.write(item + '\n')
	f.close()


def ngrams(tokenStrings, n):

	dimentions = {}
	ngrams = []
	countD = 0	
	for token in tokenStrings:
                i1 = 0
                i2 = n
                tokens = []
		if i2 > len(token[1]):
			tokens.append(token[1])
                while i2 <= len(token[1]):
                        tokens.append(token[1][i1:i2])
                        i1 += 1
                        i2 += 1
#               print len(tokens)
                counter = collections.Counter(tokens)
                for item in counter.keys():
			#print item
                        ngrams.append([token[0], item, counter[item]])
                        if item not in dimentions:
                                countD += 1
                                dimentions[item] = countD

	f = open('char' + str(n) + '.csv', 'w')
	for item in ngrams:
		f.write(str(objectMap[item[0]]) + ',' + str(dimentions[item[1]]) + ',' + str(item[2]) + '\n')
	f.close()

	f = open('char' + str(n) + '.clabel', 'w')
	for item in dimentions.keys():
		f.write(item + '\n')
	f.close()


stopwords = ['secondly', 'all', 'consider', 'pointing', 'whoever', 'four', 'edu', 'go', 'oldest', 'causes', 'seemed', 'whose', 'certainly', "when's", 'vs', 'young', 'presents', 'to', 'asking', 'indicate', 'those', 'th', 'under', 'sorry', "a's", 'sent', 'insofar', 'outside', 'far', 'every', 'yourselves', 'little', "we'll", 'inward', 'presented', 'did', 'turns', "they've", 'large', 'p', 'small', 'thereupon', "it'll", "i'll", 'parted', 'smaller', 'says', "you'd", 'yourself', 'second', 'further', 'even', 'what', 'appear', 'exactly', 'brief', 'anywhere', 'above', 'sup', 'new', 'rd', 'ever', 'full', "c'mon", 'respectively', 'men', 'here', 'youngest', 'let', 'groups', 'others', 'alone', 'along', "aren't", 'great', 'k', 'allows', "i'd", "weren't", "he'd", 'usually', 'whereupon', "i'm", 'changes', 'thats', 'hither', 'via', 'followed', 'besides', 'merely', 'put', 'viz', 'everybody', 'use', 'from', 'working', 'contains', 'two', 'next', 'almost', 'therefore', 'taken', 'themselves', 'thru', 'until', 'today', 'more', 'knows', 'clearly', 'becomes', 'hereby', 'it', 'downing', 'everywhere', 'particular', 'known', 'cases', 'must', 'me', 'states', 'none', 'room', 'thanks', 'f', 'this', 'work', 'itself', 'nine', 'can', 'mr', 'following', 'making', 'my', 'example', 'numbers', 'indicated', 'give', "didn't", 'near', 'high', 'indicates', 'something', 'want', 'comes', 'needs', 'end', 'turn', 'rather', 'meanwhile', 'how', 'instead', 'okay', 'y', 'may', 'after', 'containing', 'hereupon', 'such', 'man', 'a', 'third', 'whenever', 'maybe', 'appreciate', 'q', 'ones', 'so', 'specifying', 'allow', 'keeps', 'order', "that's", 'six', 'help', "don't", 'furthering', 'indeed', 'over', 'mainly', 'soon', 'years', 'ended', 'through', 'looks', 'still', 'its', 'before', 'beside', 'group', 'thank', "he's", 'somewhere', 'interesting', 'actually', 'better', 'willing', 'differently', 'thanx', 'ours', 'might', "haven't", 'then', 'non', 'good', 'somebody', 'greater', 'thereby', 'downs', "you've", 'they', 'not', 'now', 'nor', 'gets', 'hereafter', 'always', 'reasonably', 'qv', 'whither', 'l', 'each', 'went', 'side', "isn't", 'mean', 'everyone', 'doing', 'eg', "we'd", 'year', 'our', 'beyond', 'out', 'try', 'opened', 'twice', 'furthermore', 'since', 'looking', 're', 'seriously', "shouldn't", "they'll", 'got', 'cause', 'shows', 'regards', 'howbeit', 'turning', "you're", 'differ', 'quite', 'que', 'members', 'ask', 'anyhow', 'wanted', 'g', 'could', 'needing', 'tries', 'keep', 'thing', 'place', 'w', 'ltd', 'hence', 'onto', 'think', 'first', 'already', 'seeming', 'thereafter', 'number', 'welcome', 'one', 'done', 'another', 'open', 'awfully', "doesn't", 'given', "there's", 'needed', 'ordering', 'inasmuch', 'least', 'name', 'anyone', 'their', 'too', 'gives', 'interests', 'mostly', 'behind', 'nobody', 'took', 'immediate', 'part', 'somewhat', 'kept', 'believe', 'herself', 'than', "here's", 'kind', 'b', 'unfortunately', 'showed', 'gotten', 'older', 'likely', 'nevertheless', 'r', 'were', 'toward', 'anyways', 'and', 'sees', 'turned', 'few', 'say', 'unlikely', 'have', 'need', 'seen', 'seem', 'obviously', 'saw', 'orders', 'relatively', 'zero', "she'd", 'latter', 'that', 'able', 'aside', 'thorough', 'also', 'take', 'which', 'wanting', 'wonder', 'sure', 'unless', 'shall', 'knew', 'wells', "where's", 'most', 'eight', 'amongst', 'nothing', 'why', 'parting', 'sub', 'especially', 'noone', 'later', 'm', 'mrs', "you'll", 'points', 'normally', 'fact', 'saying', 'particularly', 'show', 'anyway', 'ending', 'find', 'fifth', 'trying', 'state', "won't", 'should', 'only', 'going', 'specify', 'herein', 'pointed', 'do', 'his', 'goes', 'get', 'hopefully', 'overall', 'selves', 'truly', 'self', "they'd", 'cannot', 'longest', 'despite', 'during', 'him', 'nearly', 'areas', 'regarding', 'course', 'h', "wasn't", 'she', 'contain', 'x', 'where', 'greetings', 'ignored', 'theirs', 'we', "hasn't", 'see', 'are', 'best', 'definitely', 'said', 'ways', 'away', 'currently', 'please', 'enough', 'smallest', 'various', 'between', 'probably', 'neither', 'across', 'tried', 'available', 'ends', 'never', 'useful', 'opening', 'however', 'come', 'both', 'c', 'last', 'many', "wouldn't", 'thence', 'according', 'against', 'etc', 's', 'became', 'faces', 'whole', 'asked', "can't", 'otherwise', 'among', 'liked', 'co', 'afterwards', 'point', 'seems', 'whatever', 'furthered', "hadn't", "couldn't", 'associated', 'throughout', 'considering', 'furthers', 'described', 'puts', "it's", 'three', 'been', 'whom', 'much', 'wherein', 'interest', 'hardly', "it'd", 'wants', 'corresponding', 'beforehand', "what's", 'else', 'worked', 'an', 'hers', 'former', 'present', 'case', 'myself', 'novel', 'look', 'these', 'nd', 'value', 'n', 'will', 'while', 'theres', 'seven', 'would', 'backing', 'wherever', 'is', 'thus', 'them', 'cant', 'someone', 'in', 'ie', 'if', 'different', 'perhaps', 'things', 'make', 'same', 'any', 'member', 'parts', 'appropriate', 'several', 'higher', 'used', 'upon', 'uses', "he'll", 'thoughts', 'off', 'whereby', 'largely', 'i', 'com', 'well', 'anybody', 'finds', 'thought', 'without', 'greatest', 'thoroughly', 'very', 'the', 'yours', 'latest', 'lest', "she'll", 'newest', 'just', 'less', 'being', 'when', 'presumably', 'rooms', 'facts', 'regardless', 'yes', 'lately', 'yet', 'unto', "we've", 'had', 'except', 'sometimes', 'lets', 'interested', 'inner', 'has', 'ought', 'gave', "t's", 'around', 'big', 'showing', "who's", 'possible', 'early', 'five', 'know', 'using', 'apart', 'like', 'necessary', 'd', "ain't", 'follows', 't', 'fully', 'become', 'works', 'grouping', 'therein', "why's", 'because', 'old', 'often', 'namely', 'downwards', 'some', 'back', 'oh', 'towards', 'moreover', 'thinks', "shan't", 'happens', 'for', 'though', 'per', 'everything', 'does', 'provides', 'tends', 'either', 'be', 'who', 'sensible', 'seconds', 'nowhere', 'although', 'by', 'on', 'about', 'goods', 'ok', 'asks', 'anything', 'getting', 'of', 'o', "mustn't", 'whence', 'plus', 'whereafter', 'consequently', 'or', 'seeing', 'own', 'formerly', 'into', 'within', 'down', 'beings', 'right', "c's", 'your', 'forth', "how's", 'her', 'area', 'downed', 'there', 'long', 'accordingly', "we're", 'way', 'was', 'opens', 'himself', 'elsewhere', "i've", 'becoming', 'but', 'somehow', 'hi', 'newer', 'highest', 'with', 'he', "they're", 'made', 'places', 'whether', 'wish', 'j', 'up', 'us', 'tell', 'placed', 'below', 'un', 'problem', 'z', 'clear', 'ex', 'gone', 'v', 'ordered', 'certain', 'specified', 'am', 'general', 'as', 'sometime', 'at', 'et', 'face', 'inc', 'again', 'entirely', 'no', 'whereas', 'generally', 'backs', 'ourselves', 'grouped', 'other', 'latterly', 'you', 'really', 'concerning', 'felt', 'problems', "let's", 'important', 'serious', 'sides', 'began', 'younger', 'e', 'longer', 'came', 'backed', 'together', 'hello', 'u', 'presenting', "she's", 'evenly', 'having', 'once']

#stopwords = []

#path = '/home/rohan/Clustering/20_newsgroups'
#path = 'mini_newsgroups'
path = '20_newsgroups'

files = glob(path+'/*/*')
#print len(files)
count = 0
countR = 0
countL = 0
tokenStrings = []
objectMap = {}
newsGroupList = []

ID = 0

#files = ['20_newsgroups/comp.graphics/38382', '/home/rohan/Clustering/20_newsgroups/alt.atheism/51060', '/home/rohan/Clustering/20_newsgroups/sci.med/58053', '/home/rohan/Clustering/20_newsgroups/sci.med/58813']
#files = ['/home/rohan/Clustering/20_newsgroups/comp.graphics/37940']
for filepath in files:
	with open(filepath) as f:
		fileContent = ''
		subjectFlag = 0
		linesFlag = 0
		for line in f:			
			line = line.lower()
			if subjectFlag == 0 and 'subject:' in line :
				subjectFlag = 1
				#Check if the posting is reply. 
				if 're:' in  line:
					countR += 1
					break	
				fileContent = line.split('subject:')[1].strip('\n').lstrip() + fileContent
				count += 1
#				print filepath
			
			if 'lines' == line.split(':')[0]:
#				print filepath
				linesFlag = 1
				countL += 1
#				print line
				numberOfLines = int(line.split(':')[1].strip('\n'))
				fileContent = fileContent + ' ' + tail(filepath, numberOfLines)
							
			if linesFlag == 1 and subjectFlag == 1:				
				fileContent = re.sub('[^a-zA-Z0-9]', r' ',re.sub('[^\x00-\x7f]',r'',fileContent))
				fileContent = fileContent.lower()
				fileContent = re.sub(' +', r' ', fileContent)
				tokenString = ' '.join(fileContent.split())
				tokenString = re.sub(r'\b\d+\b', '', tokenString)
				
				#StopWords Removal
				tokens = tokenString.split();
				for word in tokenString.split():
					if word in stopwords:
						tokens.remove(word)				
				tokenString = ' '.join(tokens)
				###
	
				if tokenString != '':
					#print ID, tokenString
					tokenStrings.append([filepath, tokenString])
					ID += 1
					objectMap[filepath] = ID
					newsGroupList.append([ID, filepath.split('/')[-2], filepath.split('/')[-1]])
				
#				print counter
#				print tokens
				break

newsGroup(newsGroupList)
newsGroupList = []
bagOfWords(tokenStrings)
ngrams(tokenStrings, 3)
ngrams(tokenStrings, 5)
ngrams(tokenStrings, 7)
