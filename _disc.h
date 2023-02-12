/*
Работа с диском
*/
#ifndef FILE_array
#include "_array.h"
#endif
#ifndef FILE_string
#include "_string.h"
#endif

namespace d {
	#include <sys/types.h>
	#include<sys\stat.h>
	#include <dirent.h>
	class Info {
	private:
		STRING way;
		struct stat status;
	public:
		Info(STRING way){
			this->way=way;
			stat(*way, &this->status);
		}
		STRING Mode(){
			STRING res;
			switch (this->status.st_mode & S_IFMT){
				case S_IFREG:	res="file";break;
				case S_IFIFO:		res="fifo";break;
				case S_IFDIR:		res="folder";break;
				case S_IFBLK:	res="block device";break;
				case S_IFCHR:	res="character device";break;
				//case S_IFLNK:	res="symbolic link";break;
				//case S_IFSOCK:	res="socket";break;
				default:				res="unknown";
			}
			return res;
		}
		STRING Way(){return this->way;}
		INT_S Uid(){return this->status.st_uid;}
		INT_S Gid(){return this->status.st_gid;}
		INT_W Ino(){return this->status.st_ino;}
		INT_L Dev(){return this->status.st_dev;}
		INT_B Size(){return this->status.st_size;}
		INT_L Rdev(){return this->status.st_rdev;}
		INT_S Nlink(){return this->status.st_nlink;}
		DATETIME Opening(){return DATETIME(this->status.st_atime);}
		DATETIME Creating(){return DATETIME(this->status.st_ctime);}
		DATETIME Modification(){return DATETIME(this->status.st_mtime);}
	};
	Associative scan(CHARS(pth)){
		Associative res;
		struct dirent *dt;
		DIR *dir=opendir(pth);
		if(dir!=NULL){
			Array<STRING> way;
			Associative *fls=new Associative;
			Array<STRING> *fld=new Array<STRING>;
			way.Put(pth);way.Put("");
			while((dt=readdir(dir))!=NULL){
				way[2]=dt->d_name;
				Info inf(s::text(way));
				STRING mode=inf.Mode();
				if(way[2]!="." && way[2]!=".."){
					if(mode=="folder")(*fld).Put(way[2]);
					else if(mode=="file"){
						Associative *ifl=new Associative;
						(*ifl)["size"]=inf.Size();
						(*ifl)["opening"]=inf.Opening();
						(*ifl)["creating"]=inf.Creating();
						(*ifl)["modification"]=inf.Modification();
						(*fls)[way[2]]=*ifl;
					}
				}
			}
			if(*fld)res["folders"]=*fld;
			if(*fls)res["files"]=*fls;
			closedir(dir);
		}
		return res;
	}
}

#define FILE_disc
