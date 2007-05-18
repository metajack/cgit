#ifndef CGIT_H
#define CGIT_H


#include <git-compat-util.h>
#include <cache.h>
#include <grep.h>
#include <object.h>
#include <tree.h>
#include <commit.h>
#include <tag.h>
#include <diff.h>
#include <diffcore.h>
#include <refs.h>
#include <revision.h>
#include <log-tree.h>
#include <archive.h>
#include <xdiff/xdiff.h>


/*
 * The valid cgit repo-commands
 */
#define CMD_LOG      1
#define CMD_COMMIT   2
#define CMD_DIFF     3
#define CMD_TREE     4
#define CMD_VIEW     5
#define CMD_BLOB     6
#define CMD_SNAPSHOT 7

typedef void (*configfn)(const char *name, const char *value);
typedef void (*filepair_fn)(struct diff_filepair *pair);
typedef void (*linediff_fn)(char *line, int len);

struct cacheitem {
	char *name;
	struct stat st;
	int ttl;
	int fd;
};

struct repoinfo {
	char *url;
	char *name;
	char *path;
	char *desc;
	char *owner;
	char *defbranch;
	char *module_link;
	int snapshots;
	int enable_log_filecount;
	int enable_log_linecount;
};

struct repolist {
	int length;
	int count;
	struct repoinfo *repos;
};

struct commitinfo {
	struct commit *commit;
	char *author;
	char *author_email;
	unsigned long author_date;
	char *committer;
	char *committer_email;
	unsigned long committer_date;
	char *subject;
	char *msg;
};

struct taginfo {
	char *tagger;
	char *tagger_email;
	int tagger_date;
	char *msg;
};

extern const char cgit_version[];

extern struct repolist cgit_repolist;
extern struct repoinfo *cgit_repo;
extern int cgit_cmd;

extern char *cgit_root_title;
extern char *cgit_css;
extern char *cgit_logo;
extern char *cgit_logo_link;
extern char *cgit_module_link;
extern char *cgit_virtual_root;
extern char *cgit_script_name;
extern char *cgit_cache_root;

extern int cgit_nocache;
extern int cgit_snapshots;
extern int cgit_enable_log_filecount;
extern int cgit_enable_log_linecount;
extern int cgit_max_lock_attempts;
extern int cgit_cache_root_ttl;
extern int cgit_cache_repo_ttl;
extern int cgit_cache_dynamic_ttl;
extern int cgit_cache_static_ttl;
extern int cgit_cache_max_create_time;

extern int cgit_max_msg_len;
extern int cgit_max_repodesc_len;
extern int cgit_max_commit_count;

extern int cgit_query_has_symref;
extern int cgit_query_has_sha1;

extern char *cgit_querystring;
extern char *cgit_query_repo;
extern char *cgit_query_page;
extern char *cgit_query_search;
extern char *cgit_query_head;
extern char *cgit_query_sha1;
extern char *cgit_query_sha2;
extern char *cgit_query_path;
extern char *cgit_query_name;
extern int   cgit_query_ofs;

extern int htmlfd;

extern int cgit_get_cmd_index(const char *cmd);
extern struct repoinfo *cgit_get_repoinfo(const char *url);
extern void cgit_global_config_cb(const char *name, const char *value);
extern void cgit_repo_config_cb(const char *name, const char *value);
extern void cgit_querystring_cb(const char *name, const char *value);

extern int chk_zero(int result, char *msg);
extern int chk_positive(int result, char *msg);

extern int hextoint(char c);

extern void *cgit_free_commitinfo(struct commitinfo *info);

extern int cgit_diff_files(const unsigned char *old_sha1,
			   const unsigned char *new_sha1,
			   linediff_fn fn);

extern void cgit_diff_tree(const unsigned char *old_sha1,
			   const unsigned char *new_sha1,
			   filepair_fn fn);

extern void cgit_diff_commit(struct commit *commit, filepair_fn fn);

extern char *fmt(const char *format,...);

extern void html(const char *txt);
extern void htmlf(const char *format,...);
extern void html_txt(char *txt);
extern void html_ntxt(int len, char *txt);
extern void html_attr(char *txt);
extern void html_hidden(char *name, char *value);
extern void html_link_open(char *url, char *title, char *class);
extern void html_link_close(void);
extern void html_filemode(unsigned short mode);

extern int cgit_read_config(const char *filename, configfn fn);
extern int cgit_parse_query(char *txt, configfn fn);
extern struct commitinfo *cgit_parse_commit(struct commit *commit);
extern struct taginfo *cgit_parse_tag(struct tag *tag);
extern void cgit_parse_url(const char *url);

extern char *cache_safe_filename(const char *unsafe);
extern int cache_lock(struct cacheitem *item);
extern int cache_unlock(struct cacheitem *item);
extern int cache_cancel_lock(struct cacheitem *item);
extern int cache_exist(struct cacheitem *item);
extern int cache_expired(struct cacheitem *item);

extern char *cgit_repourl(const char *reponame);
extern char *cgit_pageurl(const char *reponame, const char *pagename,
			  const char *query);

extern void cgit_print_error(char *msg);
extern void cgit_print_date(unsigned long secs);
extern void cgit_print_docstart(char *title, struct cacheitem *item);
extern void cgit_print_docend();
extern void cgit_print_pageheader(char *title, int show_search);
extern void cgit_print_snapshot_start(const char *mimetype,
				      const char *filename,
				      struct cacheitem *item);

extern void cgit_print_repolist(struct cacheitem *item);
extern void cgit_print_summary();
extern void cgit_print_log(const char *tip, int ofs, int cnt, char *grep, char *path);
extern void cgit_print_view(const char *hex, char *path);
extern void cgit_print_blob(struct cacheitem *item, const char *hex, char *path);
extern void cgit_print_tree(const char *rev, const char *hex, char *path);
extern void cgit_print_commit(const char *hex);
extern void cgit_print_diff(const char *head, const char *old_hex, const char *new_hex,
			    char *path);
extern void cgit_print_snapshot(struct cacheitem *item, const char *hex,
				const char *format, const char *prefix,
				const char *filename);

#endif /* CGIT_H */
