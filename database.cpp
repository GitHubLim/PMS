#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32.lib")


#include <iostream>
#include <string>

//MySQL 관련 헤더파일
#include <my_global.h>
#include <mysql.h>

using namespace std;

int main(int argc, char **argv)
{

	string str_query;
	string str_input_parking_id;
	string str_input_parking_level;
	int ninput_parking_id;
	int ninput_parking_level;

	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	int query_stat;


	char *server = "localhost";
	char *user = "root";
	char *password = "rkdxorn"; //root계정 비밀번호
	char *database = "test"; //생성한 데이터베이스
	int port = 3307; //포트번호

	conn = mysql_init(NULL);

	/* DB 접속 */
	if (!mysql_real_connect(conn, server,
		user, password, database, port, NULL, 0)) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}

	//값 보내기
	query_stat = mysql_query(conn, "select * from parking");
	if (query_stat != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(conn));
		return 1;
	}
	
	/* send SQL query */
	//parking 테이블 모든 자료 검색
	if (mysql_query(conn, str_query.c_str())) {
		fprintf(stderr, "%s ", mysql_error(conn));
		exit(1);
	}
	res = mysql_store_result(conn);

	/* close connection */
	mysql_free_result(res);
	mysql_close(conn);
	return 0;
}
