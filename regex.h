#ifndef REGEX
#define REGEX

#include "queue.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

#define res(value) push(res,value)
#define islower(c)  ('a' <= c && c <= 'z')
#define isupper(c)  ('A' <= c && c <= 'Z')
#define isdigit(c)	('0' <=c && c<='9')
#define isalpha(c) isupper(c)||islower(c)
#define isblank(c) ('\t' <= c && c <= '\r')||c==' '
int iscomplete(char* regex, char* pattern, int regex_index, int* num_res_delete);
int size(char* arr);
Queue* res; //申明全局的Queue用于存储匹配到的字符
char* bracket_content; //用于存[]中括号内容

int equal(char regex_char, char pattern_char) {
	int flag = 0;
	if (regex_char == '.') flag = 1;
	else if (regex_char == pattern_char) flag = 1;
	else if (regex_char == '\a') flag = isalpha(pattern_char);
	else if (regex_char == '\b') flag = isdigit(pattern_char);
	else if (regex_char == '\v') flag = isblank(pattern_char);
	else if (regex_char == ']') {
		int exist_[MAXSIZE]; int _index = 0;
		int size_bracket = size(bracket_content);
		for (int i = 0; i < size_bracket; i++) {
			if (equal(bracket_content[i], pattern_char)) flag = 1;
			if (bracket_content[i] == '-') exist_[_index++] = i;
		}
		for (int i = 0; i < _index; i++) {
			if (bracket_content[exist_[i] - 1] <= pattern_char && pattern_char <= bracket_content[exist_[i] + 1]) flag = 1;
		}
	}
	return flag;
}


int len(char* regex, int regex_index) {
	//算最小长度从regex_index开始的
	int count = 0;
	int i = 0;
	for (i = regex_index;regex[i]!='\0'; i++) {
		if (regex[i] == '[') {
			while (regex[i] != ']') i++;
		}
		if (regex[i] == '*' || regex[i] == '?') count -= 2;
		if (regex[i] == '+') count -= 1;
		count++;
	}
	return count;
}


int size(char* arr) {
	//算指针指向对象的数组长度
	int count = 0;
	while (*(++arr) != '\0') count++;
	return count+1;
}
int index_equal_zero(char* regex, int regex_index) {
	int count = 0;
	if (regex_index == 0) return 1;
	for (int i = 0; i <= regex_index; i++) {
		if (regex[i] == '[') while (regex[++i] != ']');
		count++;
		if (regex[i] == '+') count -= 1;
		if (regex[i] == '*' || regex[i] == '?') count -= 2;
	}
	return count == 1;
}

//匹配*号，不带括号
int match_star(char prev_char, char* pattern, int pattern_index) {
	/*
		prev_char:*号前一个字符
		pattern:匹配的目标串
		pattern_index:待匹配目标串的当前位置索引
		return:返回失配位置索引
	*/

	//如果可以匹配就一直匹配下去
	while (pattern[pattern_index] != '\0' && equal(prev_char, pattern[pattern_index])) {
		res(pattern[pattern_index++]);
	}

	return pattern_index;
}

int match_add(char prev_char, char* pattern, int pattern_index) {
	/*
	prev_char:?号前一个字符
	pattern:匹配的目标串
	pattern_index:待匹配目标串的当前位置索引
	return:返回失配位置索引
	*/
	while (pattern[pattern_index] != '\0' && equal(prev_char, pattern[pattern_index])) {
		res(pattern[pattern_index++]);
	}

	return pattern_index;

}

int match_question_mark(char prev_char, char* pattern, int pattern_index) {
	/*
	prev_char:?号前一个字符
	pattern:匹配的目标串
	pattern_index:待匹配目标串的当前位置索引
	return:返回失配位置索引
	*/

	if (equal(prev_char, pattern[pattern_index])) res(pattern[pattern_index++]);
	return pattern_index;

}
void re_search(char* regex, char* pattern) {
	/*
	 *regex:正则表达式
	 *pattern:匹配的目标串
	 *检查全部目标串，直到目标串检查完，还没匹配成功就退出
	 */

	int regex_index = 0;
	int pattern_index = 0;
	int len_regex = len(regex, 0);
	int len_pattern = len(pattern, 0);
	//如果进入函数发现正则式子比，目标串长则直接退出
	if (len_regex > len_pattern) return;

	//开始匹配
	while (pattern[pattern_index] != '\0') {
		//正则式子串匹配完，退出
		if (regex[regex_index] == '\0' && length(res) != 0) return;
		//含有*号
		else if (regex[regex_index + 1] == '*') {
			pattern_index = match_star(regex[regex_index++], pattern, pattern_index);
			regex_index++;
			int prev_char_index = regex_index - 2;

			/*
				当*号后面与*号前面相同时，应当一直匹配到尾
				regex:ab*bbbbbb    pattern:abbbbbbbbbbbbb
				这时候除了匹配b*外，应该将regex_index移动到尾
			*/
			while (regex[prev_char_index] == regex[regex_index]) {regex_index++;}
		}
		else if (regex[regex_index + 1] == '+') {
			int match_add_return = match_add(regex[regex_index++], pattern, pattern_index);

			//第一个就失配
			if (match_add_return == pattern_index) {
				//尾部失配，直接退出
				int size_pattern=size(pattern);
				if (index_equal_zero(regex, regex_index)) pattern_index++;
				else if (pattern_index == size_pattern - 1) {clear(res); regex_index = 0; break;}
				regex_index = 0; clear(res);
			}
			else {
				pattern_index = match_add_return;
				regex_index++;
				int prev_char_index = regex_index - 2;

				/*
				当*号后面与*号前面相同时，应当一直匹配到尾
				regex:ab+bbbbbb    pattern:abbbbbbbbbbbbb
				这时候除了匹配b+外，应该将regex_index移动到尾
				*/
				//
				while (regex[prev_char_index] == regex[regex_index]) {regex_index++;}
			}

		}
		else if (regex[regex_index + 1] == '?') {
			pattern_index = match_question_mark(regex[regex_index++], pattern, pattern_index);
			regex_index++;
		}

		else if (regex[regex_index] == '[') {
			char temp[MAXSIZE] = {'\0'}; int i = 0;
			regex_index++;
			while (1) {
				if (regex[regex_index] == ']') break;
				temp[i++] = regex[regex_index++];
			}
			bracket_content = temp;
		}

		else if (!equal(regex[regex_index], pattern[pattern_index])) {
			int size_pattern=size(pattern);
			if (index_equal_zero(regex, regex_index)) pattern_index++;
			else if (pattern_index == size_pattern - 1) {clear(res); regex_index = 0; break;}
			regex_index = 0; clear(res);
		}
		else if (equal(regex[regex_index], pattern[pattern_index])) {
			res(pattern[pattern_index]);
			pattern_index++;
			regex_index++;
		}

	}

	Queue* temp = (Queue*)malloc(sizeof(Queue));
	create(temp);
	copy(res, temp);
	clear(res);
	int num_res_delete = 0;
	if (!iscomplete(regex, pattern, regex_index, &num_res_delete)) {
		clear(temp);
	}
	copy(temp, res);
	num_res_delete = (num_res_delete>=length(res)?length(res):num_res_delete);
	for (int i = 0; i < num_res_delete ; i++) pop(res);
	destroy(temp);

}
int re_match(char* regex, char* pattern, int regex_index, int pattern_index) {
	/*
	 *regex:正则表达式
	 *pattern:匹配的目标串
	 ×match从头开始匹配，一失配就退出
	 */
	int len_regex = len(regex, 0);
	int len_pattern = len(pattern, 0);
	//如果进入函数发现正则式子比，目标串长则直接退出
	if (len_regex > len_pattern) return 0;

	//开始匹配
	while (pattern[pattern_index] != '\0') {
		//正则式子串匹配完，退出
		if (regex[regex_index] == '\0') return 1;
		//含有*号
		else if (regex[regex_index + 1] == '*') {
			pattern_index = match_star(regex[regex_index++], pattern, pattern_index);
			regex_index++;
			int prev_char_index = regex_index - 2;

			/*
				当*号后面与*号前面相同时，应当一直匹配到尾
				regex:ab*bbbbbb    pattern:abbbbbbbbbbbbb
				这时候除了匹配b*外，应该将regex_index移动到尾
			*/
			while (regex[prev_char_index] == regex[regex_index]) {regex_index++;}
		}
		else if (regex[regex_index + 1] == '+') {
			int match_add_return = match_add(regex[regex_index++], pattern, pattern_index);

			//第一个就失配
			if (match_add_return == pattern_index) {
				clear(res);
				break;
			}
			else {
				pattern_index = match_add_return;
				regex_index++;
				int prev_char_index = regex_index - 2;

				/*
				当*号后面与*号前面相同时，应当一直匹配到尾
				regex:ab+bbbbbb    pattern:abbbbbbbbbbbbb
				这时候除了匹配b+外，应该将regex_index移动到尾
				*/
				//
				while (regex[prev_char_index] == regex[regex_index]) {regex_index++;}
			}

		}
		else if (regex[regex_index + 1] == '?') {
			pattern_index = match_question_mark(regex[regex_index++], pattern, pattern_index);
			regex_index++;
		}
		else if (regex[regex_index] == '[') {
			char temp[MAXSIZE] = {'\0'}; int i = 0;
			regex_index++;
			while (1) {
				if (regex[regex_index] == ']') break;
				temp[i++] = regex[regex_index++];
			}
			bracket_content = temp;
		}
		else if (!equal(regex[regex_index], pattern[pattern_index])) {
			clear(res);
			break;
		}
		else if (equal(regex[regex_index], pattern[pattern_index])) {
			res(pattern[pattern_index]);
			pattern_index++;
			regex_index++;
		}

	}
	if (length(res) == 0) return 0;

	Queue* temp = (Queue*)malloc(sizeof(Queue));
	create(temp);
	copy(res, temp);
	clear(res);
	int num_res_delete = 0;
	int flag = iscomplete(regex, pattern, regex_index, &num_res_delete);
	if (!flag) {
		clear(temp);
	}
	copy(temp, res);
	destroy(temp);
	return flag;

}

int iscomplete(char* regex, char* pattern, int regex_index, int* num_res_delete) {
	//判断regex串是否匹配完
	int flag = 0;
	int remain_regex_char_len = len(regex, regex_index);
	int pattern_start_index = size(pattern) - remain_regex_char_len;


	if (remain_regex_char_len == 0) return 1;
	/*
	*如果前一个数是加而后一个数是不可忽略的，哪个只能是两个模式相同也就是epual的上
	*
	*/
	if (regex[regex_index - 1] == '+') {
		char right_regex_char = 0;
		char left_regex_char = 0;
		int i = regex_index - 2;
		while (i && (regex[i] == '[' || regex[i] == ']')) i--;
		left_regex_char = regex[i];
		int j = regex_index;
		while (regex[j] != '\0' && (regex[j] == '[' || regex[j] == ']')) j++;

		int tag = 0; //记录后一个字符是不是可以忽略，0：可以忽略,1:不可以忽略
		if (regex[regex_index] == '[') {
			int k = regex_index;
			while (regex[k] != '\0' && regex[k] != ']') k++;
			if (regex[k + 1] != '*' && regex[k + 1] != '?') tag = 1;
		}
		else {
			if (regex[regex_index + 1] != '*' && regex[regex_index + 1] != '?') tag = 1;
		}
		right_regex_char = regex[j];
		if (!equal(regex[i], regex[j]) && tag == 1) return 0;
	}


	if (remain_regex_char_len == 1) {
		int temp = 0;
		if (regex[regex_index - 2] == '\b' || regex[regex_index - 2] == '\a' || regex[regex_index - 2] == '\v') temp = (regex[regex_index - 2] == regex[regex_index]);
		flag = (regex[regex_index] == '\0' || regex[regex_index] == '*' || regex[regex_index] == '?' || temp);
	}
	if (remain_regex_char_len == 2) {
		flag = (regex[regex_index + 1] == '*' || regex[regex_index + 1] == '?');
	}
	//通过反向再次匹配来检验是匹配完
	for (int i = 0; i <= pattern_start_index; i++) {
		int tag = re_match(regex, pattern, regex_index, pattern_start_index - i);
		if (tag) {
			flag = 1;
			int temp = remain_regex_char_len + i - length(res);
			if (length(res) > remain_regex_char_len) {flag = 0; break;}
			*num_res_delete = temp;
			break;
		}
	}

	return flag;
}



#endif
