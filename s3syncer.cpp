#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

int main()
{
	char dir[100];
	string new_folder;
	string syncdir;
    	string bkpjob;
	char response[20];
	string syncjob;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
    	cout<<"+++++++++++++++S3 Syncer for Linux+++++++++++++++"<<endl;
	cout<<"+++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
	cout<<"Make sure you have installed and configured s3cmd.  If not, run $sudo apt-get install s3cmd"<<endl;
	cout<<"If you have not installed s3cmd and crontab, type 'exit', otherwise:"<<endl;
    	cout<<"Please specify the location of the folder you would like to sync (ie. $HOME/...): ";
    	cin>>dir;
   	if((strcmp(dir,"exit")==0))
	{
		cout<<"Now exiting..."<<endl;
	}
	else
	{
		cout<<"Specify the bucket in S3 for storing this directory: ";
    		cin>>new_folder;
       		std::string command="s3cmd mb s3://"+new_folder;
		system((const char*)command.c_str());
		cout<<"Input name of sync job: ";
		cin>>bkpjob;
		std::string bkpname="."+bkpjob+".cpp";
		cout<<"Enter the name of the folder the sync job is stored (should be your current directory name): ";
		cin>>syncdir;
		ofstream output_file;
		output_file.open(bkpname.c_str());
		output_file<<"#include <iostream>"<<endl;
		output_file<<"#include <string>"<<endl;
		output_file<<"#include <cstdlib>"<<endl;
		output_file<<"#include <cstdio>"<<endl;
		output_file<<"using namespace std;"<<endl;
		output_file<<"int main()"<<endl;
		output_file<<"{"<<endl;
		output_file<<"system(\"s3cmd sync "<<dir<<" s3://"<<new_folder<<"\");"<<endl;
		output_file<<"}"<<endl;
		std::string command1="g++ "+bkpname+" -o ."+bkpjob;
		system((const char*)command1.c_str());
		std::string command2="./."+bkpjob;
		system((const char*)command2.c_str());
		cout<<"Folder now synced!"<<endl;
		cout<<"Would you like to automatically sync folder whenever files are changed or added?: ";
		cin>>response;
		if(strcmp(response,"yes")==0)
		{
			fstream file;
			file.open("syncjobs.txt",ios::out | ios::in | ios::app);
			file<<"* * * * * ./"+syncdir+"/."<<bkpjob<<endl;
			system("crontab syncjobs.txt");
			cout<<"Sync added, to check folders that are synced, use $sudo crontab -l"<<endl;
			cout<<"Now exiting..."<<endl;
		}
		else if(strcmp(response,"no")==0)
		{
			cout<<"Now exiting..."<<endl;
			
		}	
	}
} 
