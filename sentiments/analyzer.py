from nltk.tokenize import TweetTokenizer

class Analyzer():
    """Implements basic sentiment analysis."""

    def __init__(self, positive, negative):
        
        #set filenames
        self.positivefilename = positive
        self.negativefilename = negative
        
        #open files containing positive/negative words
        self.positivefp = open(self.positivefilename)
        if self.positivefp is None:
            print("Error opening " + self.positivefilename)
        self.negativefp = open(self.negativefilename)
        if self.negativefp is None:
            print("Error opening " + self.negativefilename)
        
        #create sets of positive/negative words, ; denotes comment in .txt file
        self.positivewords = set(x.strip("\n") for x in self.positivefp.readlines() if ";" not in x and x is not "\n")
        self.negativewords = set(x.strip("\n") for x in self.negativefp.readlines() if ";" not in x and x is not "\n")

    def analyze(self, text):
        
        sentiment_score = 0
        
        #TweetTokenizer returns list of words in tweet
        tokenizer = TweetTokenizer(preserve_case=False, reduce_len=True, strip_handles=True)
        
        for token in tokenizer.tokenize(text):
            if token in self.positivewords:
                sentiment_score += 1
            elif token in self.negativewords:
                sentiment_score -= 1
        
        return sentiment_score
