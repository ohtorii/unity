
class kind{
public:
	
}

class common:public kind{
public:
	virtual void preview(vector<candidate> candidates);
	virtual void append(vector<candidate> candidates);
}

//=============================================================================
//sources
//=============================================================================
class source{
public:
	virtual kind* default_kind();
	virtual bool gather_candidates(vector<candidate>&output);
};

class file_mru:source{
public:
	virtual kind* default_kind(){
		return new file_mru_kind;
	}
	virtual bool gather_candidates(vector<candidate>&output){
		output.append(...);
	}
	
	/* previewメソッドを上書き
	*/
	class file_mru_kind:common{
	public:
		virtual void preview(vector<candidate>&output){
		}
	}
}
