

#ifndef __SCORE__

class Score
{
public:
	Score();
	~Score();

	static void plusScore(int value);
	static int getScore();
private:
	static int _score;
};



#endif // !__SCORE__
