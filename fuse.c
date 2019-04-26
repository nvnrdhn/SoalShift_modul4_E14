/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall `pkg-config fuse --cflags --libs` -DHAVE_SETXATTR fusexmp.c -o fusexmp
*/

#define FUSE_USE_VERSION 28

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#elif __APPLE__
#define _GNU_SOURCE
#endif

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/xattr.h>
// #include <sys/attr.h>
#include <sys/param.h>
#include <pwd.h>
#include <grp.h>
#include<syslog.h>

char fix[] = "/home/somi/shift4";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = lstat(fdir, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = access(fdir, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;

	res = readlink(path, buf, size - 1);
	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			   off_t offset, struct fuse_file_info *fi)
{
	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	dp = opendir(fdir);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		char temp[300];
		memset(temp, 0, sizeof(temp));
		sprintf(temp, "%s/%s", fdir, de->d_name);
		struct stat st;
		struct group *grp;
		struct passwd *pwd;
		memset(&st, 0, sizeof(st));
		stat(temp, &st);
		grp = getgrgid(st.st_gid);
		pwd = getpwuid(st.st_uid);
		// openlog("fuse", LOG_PID, LOG_USER);
		// syslog(LOG_INFO, "%s", temp);
		// closelog();
		if (strcmp(grp->gr_name, "rusak") == 0 && (strcmp(pwd->pw_name, "chipset") == 0 || strcmp(pwd->pw_name, "ic_controller") == 0) && !(st.st_mode & S_IRUSR)) {
			char fm[300];
			memset(fm, 0, sizeof(fm));
			sprintf(fm, "%s/filemiris.txt", fix);
			FILE *miris = fopen(fm, "a");
			fprintf(miris, "%s GID: %d UID: %d Last Access: %s",de->d_name, st.st_gid, st.st_uid, ctime(&st.st_atime));
			remove(temp);
			fclose(miris);
			continue;
		}
		if (filler(buf, de->d_name, &st, 0))
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
	if (S_ISREG(mode)) {
		res = open(fdir, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fdir, mode);
	else
		res = mknod(fdir, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = mkdir(fdir, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = unlink(fdir);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = rmdir(fdir);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
	int res;

	res = symlink(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;

	res = rename(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_link(const char *from, const char *to)
{
	int res;

	res = link(from, to);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = chmod(fdir, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = lchown(fdir, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = truncate(fdir, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fdir, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = open(fdir, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi)
{
	int fd;
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	(void) fi;
	fd = open(fdir, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size,
			 off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	(void) fi;
	fd = open(fdir, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int res;
	char fdir[300];
	memset(fdir, 0, sizeof(fdir));
	sprintf(fdir, "%s%s", fix, path);

	res = statvfs(fdir, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
			 struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

static void* xmp_init(struct fuse_conn_info *conn)
{

	return NULL;
}


static struct fuse_operations xmp_oper = {
	.init		= xmp_init,
	.getattr	= xmp_getattr,
	.access		= xmp_access,
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.statfs		= xmp_statfs,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
