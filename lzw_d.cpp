#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct dictionary {
	string* entries = new string [4096];
	void initialize();
	void add(string);
private:
	uint16_t base_pointer;
};

void dictionary::initialize()
{
	base_pointer = 256;
	for (uint16_t c = 0; c < 256; c++)
		entries[c] = char(c);
	for (uint16_t i = 256; i < 4096; i++)
		entries[i] = "";
}

void dictionary::add(string x)
{
	if (base_pointer < 4096)
	{
		entries[base_pointer] = x;
		base_pointer++;
	}
	else
	{
		initialize();
		add(x);
	}
}


//Try with pairs of character codes as bitfields

struct code_pair {
	uint16_t a;
	uint16_t b;

	void update(char* p)
	{
		a = ((0xff & p[0]) << 4) | ((0xf0 & p[1]) >> 4);
		b = (0xff & p[2]) | ((p[1] & 0x0f) << 8);
	}
};


struct codestring {
	//Should be a reasonably nice way to store the file on the heap? For larger files you'd
	//want to do the decoding without reading the whole file into ram at once but this will
	//probs suffice for a few kb

	unsigned int len;
	code_pair* data;

	codestring(string file)
	{
		ifstream inputfile;
		inputfile.open(file, ios::binary);
		inputfile.seekg(0, inputfile.end);
		streampos size = inputfile.tellg();
		inputfile.seekg(0, ios::beg);

		len = (size * 2) / 3;

		if (size % 3)
		{
			data = new code_pair[(size / 3) + 1];
			len++;
		}
		else
			data = new code_pair[size / 3];

		//Have to handle this two tokens at a time because fstream only allows bytewise reading
		//Also know about padding so don't need to worry about that here


		//Neaten this, don't need two counters :)
		char* mini_buff = new char[3];
		unsigned int i = 0;
		while (i < size / 3) {
			inputfile.read(mini_buff, 3);
			data[i].update(mini_buff);
			i++;
		}

		if (size % 3)
		{
			inputfile.read(mini_buff, 2);
			data[size / 3].a = ((mini_buff[0] & 0x0f) << 8) | (mini_buff[1] & 0xff);
		}

		delete[] mini_buff;

		inputfile.close();
	}

	uint16_t at(int index)
	{
		if ((len % 2) && (index == len - 1))
			return data[(index / 2)].a;
		else if ((index < 0) || ((index >= len) && !(len % 2)))
		{
			//throw something something error handling is boring
			cout << "something broke\n";
			return 0;
		}

		else
		{

			if (index % 2)
				return data[(index / 2)].b;
			else
				return data[(index / 2)].a;
		}
	}
};


int main()
{
	string userinput,useroutput;
	cout<<"File to read: ";
	cin>>userinput;
	cout<<"File to write: ";
	cin>>useroutput;

	cout << "Importing..\n";
	codestring file(userinput);
	cout << "Loaded, working..\n";


	ofstream outfile(useroutput, ios::binary);

	dictionary d;
	d.initialize();

	//First should deffo be in the dictionary
	uint16_t prev = file.at(0);
	uint16_t next;

	//Need not initialise for first run
	string first_char;

	for (int i = 1; i < file.len - 1; i++)
	{
		next = file.at(i);

		//This relies on current one being necessarily added to dictionary by previous rounds
		outfile << d.entries[prev];
		if (d.entries[next] == "")
			first_char = d.entries[prev][0];
		else
			first_char = d.entries[next][0];

		d.add(d.entries[prev] + first_char);

		prev = next;
	}
	outfile << d.entries[prev];

	cout << "Done" << endl;

	cin.ignore();
}
