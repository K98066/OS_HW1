#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

vector<int> Infile(string filename) { // Ū�J�Ʀr 
	vector<int> numbers; // �ƦC
	ifstream file(filename); // �}���ɮ�
	
    if (file.is_open()) {
        string line;
        while (getline(file, line)) { // �v��Ū���ɮפ��e
            istringstream iss(line);
            int number;
            while (iss >> number) { // �q�C�椤Ū���Ʀr
                numbers.push_back(number); // �N�Ʀr��J�ʺA�}�C
            }
        }
        file.close(); // �����ɮ�
    }
    
    return numbers;
}

void Change_output_name(string &filename, int method_num) {
	// ���"."����m 
    size_t dotPos = filename.find_last_of('.');
    
    // ��쥿�T��m�ᴡ�J"_output" 
    if (dotPos != string::npos) {
        filename.insert(dotPos, "_output");
        
        // ���s���"."����m 
    	size_t dotPos = filename.find_last_of('.');
        
        // �[�J���
        switch(method_num) {
        	case 1:
        		filename.insert(dotPos, "1");
        		break;
        	case 2:
        		filename.insert(dotPos, "2");
        		break;
        	case 3:
        		filename.insert(dotPos, "3");
        		break;
        	case 4:
        		filename.insert(dotPos, "4");
        		break;
		}
    } 
}

void WriteFile(string &filename, vector<vector<int>> num_list, int method_num) { // �N�Ʀr�g�J�ɮ�
    
	cout << "�}�l�g�J";
	
	// ���filename�w�Foutput�ݨD 
	Change_output_name(filename, method_num);

	// �إߤ@���A�N�ƾڼg�J filename �ɮפ�
    ofstream outfile(filename);

	int list_num = 0; // �ĴX��list 
	int num_num = 0; // �ĴX��num 
	int count = 0;

    // �ˬd���O�_���\���}
    if (outfile.is_open()) {
        // �N�ƦC�����C�ӦV�q�v��g�J���
        for (const auto& vec : num_list) {
        	outfile << "Sort :" << '\n';
        	
            // �N�V�q�����C�ӼƦr�v��g�J���
            for (int number : vec) {
            	while (list_num < num_list.size()) {
            		while (num_num < num_list[list_num].size()) {	
            		    outfile << num_list[list_num][num_num] << '\n';
            		    num_num = num_num + 1;
            		    
            		    count++;
            		}
            		
            		list_num++; // ���U�@��list 
					num_num = 0; // num_num��l�� 
				}
            }
        }
        // �������y
        outfile.close();
    }
	else {
        // �p�G�L�k���}���A��ܿ��~�T��
        cout << "FAIL" << endl;
    }
    
    cout << "�g�J����"; 
}

vector<vector<int>> Split_nums (vector<int> numbers, int split_count) { // �N�Ʀr���Φ�split_count�� 
	// split_count�O���μƶq(���P�����̪�k) 
	
	vector<vector<int>> list_of_nums(split_count); // ��m�Q���Ϊ��Ʀr 
	int split_time = 0; // ���Φ�k�� = �� k-1 �M 
	int cur_num = 0; // ���evector�̪��ĴX�ӼƦr 
	
	while (cur_num < numbers.size()) {
		list_of_nums[split_time].push_back(numbers[cur_num]);
		cur_num++;
			
		if (list_of_nums[split_time].size() > (cur_num + split_count - 1) / split_count) { // �Y�O���ߪ�ܤ@�����Χ���
			// �̫�@�ӼƦC���\�����h�Ʀr�A�_�h�|�X�{���Φ�k+1�����p 
			if (split_time != split_count - 1) {
				split_time++; // �U�@��push_back�N�O�ĤG�ӼƦC�F 
			}
		}
	} // end while
	
	return list_of_nums; // �^���ɮ� 
}

void BubbleSort(vector<int> &num_list) { // �w�j�Ƨ� 
	int n = num_list.size();
	
    	// �갵bubble sort 
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (num_list[j] > num_list[j+1]) {
               	// �洫�Ʀr 
               	int temp = num_list[j];
               	num_list[j] = num_list[j+1];
               	num_list[j+1] = temp;
            }
        }
    }
}

void MergeSort(vector<vector<int>> &numbers);

void Extra_Merge(vector<vector<int>> &all_sorted_list, vector<int> right_vec) { // �B�~���X�� 
	// �J��_�ƭӼƦC�ɩҶi�檺�B�~�@���X��
	// ��k:������all_sorted_list���̫�@�ӻP�h�X���i���B�~���X�� 
	// �z�פW����]�|�O�ӦX�ִ`�����̫�@���X��
	
	vector<int> sorted_list; // �B�~���X�ֲ��ͪ��ƦC�A�̫�n���Nall_sorted_list���̫�@�ӼƦC 
	
	int i = 0, j = 0; // i �M j ���}�C�̪���?�ӼƦr 
	
	// all_sorted_list[all_sorted_list.size() - 1] �����O all_sorted_list�̳̫�@�ӼƦC 
	 
	while(i < all_sorted_list[all_sorted_list.size() - 1].size() || j < right_vec.size()) {
		// ��}�C�̪��Ʀr������A���p��push_back�isorted_list 
		if (all_sorted_list[all_sorted_list.size() - 1][i] < right_vec[j]) { 
			sorted_list.push_back(all_sorted_list[all_sorted_list.size() - 1][i]);
			i++;
		}
		else if (all_sorted_list[all_sorted_list.size() - 1][i] > right_vec[j]) {
			sorted_list.push_back(right_vec[j]);
			j++;
		}
	}
	
	all_sorted_list[all_sorted_list.size() - 1] = sorted_list;
	return;
	// �o�䤣�I�sMerge�A�ӬOreturn�^��W�@�ʪ���m�A�]�N�O�I�sMergeSort 
}

void Merge(vector<vector<int>> &all_sorted_list, // �C�@�ӥ��k��}�C�X�᪺֫�}�C�զ����}�C 
	vector<vector<int>> numbers, int left_num, int right_num) { // �X��
	
	// number = ���X�֪��ƦC
	// all_sorted_list = �X�֦n���ƦC(�ҥHfunction�@�}�l������vector) 
	
	vector<int> left_vec = numbers[left_num]; // �M�w���}�C 
	vector<int> right_vec = numbers[right_num]; // �M�w�k�}�C 

	vector<int> sorted_list; // ���k��}�C���X�֪��}�C 
	
	int i = 0, j = 0; // i �M j ���}�C�̪���?�ӼƦr 
	while(i < left_vec.size() || j < right_vec.size()) {
		// ��}�C�̪��Ʀr������A���p��push_back�isorted_list 
		if (left_vec[i] < right_vec[j]) { 
			sorted_list.push_back(left_vec[i]);
			i++;
		}
		else if (left_vec[i] > right_vec[j]) {
			sorted_list.push_back(right_vec[j]);
			j++;
		}
	}
	
	all_sorted_list.push_back(sorted_list); // �N�ƦC�n�� �l�}�C:sorted_list ��J ���}�C:all_sorted_list
	
	left_num = left_num + 1;
	right_num = right_num - 1;
	
	cout << left_num << " / " << right_num << endl;
	
	vector<vector<int>> blank_list; // ��vector�A�b�I�s�ɶǤJ�@��vector�M�ƦC�n��vector 
	
	if (left_num == right_num) { // �_�ƭӼƦC�A���H����A�w�ƥL��W�@��sorted_list�B�~�i��@��sort 
		Extra_Merge(all_sorted_list, numbers[right_num]);
	}
	if (left_num < right_num) {
		Merge(blank_list, all_sorted_list, left_num, right_num); // �o�@���|���ƦC����A�~��i�� 
	}
	
	cout << all_sorted_list.size();
	MergeSort(all_sorted_list); // �����ƦC���@���A�i��U�@�����ƦC 
}

void MergeSort(vector<vector<int>> &numbers) { // �X�ֱƧ� 
	if (numbers.size() <= 1) { // �������}�C�w�X�֦��@�Ӹg�L�ƧǪ��}�C 
		return;
	}
	
	vector<vector<int>> result_list; // �Ƨǧ����}�C 
	
	int left_num = 0;
	int right_num = numbers.size() - 1; // ��0�}�l 
	
	Merge(result_list, numbers, left_num, right_num);
	
	numbers = result_list;
}

/*
    // �O���{���}�l���檺�ɶ��I
    auto start = std::chrono::high_resolution_clock::now();

    // �b�o�̩�J�A���{���X

    // �O���{�����槹�����ɶ��I
    auto end = std::chrono::high_resolution_clock::now();

    // �p��{�����檺�ɶ��t
    std::chrono::duration<double> duration = end - start;

    // ��X����ɶ�
    std::cout << "�{������ɶ�: " << duration.count() << " ��\n";
*/ 

void Method_one (string &filename, vector<int> numbers) { // ��k�@ 
    BubbleSort(numbers); // call Bubblesort
    
    // �ѩ�WriteFile�n�D vector<vector<int>> �ɮסA�ҥH�s�W�@��
	
	vector<vector<int>> sorted_number;
	sorted_number.push_back(numbers);
    
    WriteFile(filename, sorted_number, 1); // �ͦ��ɮ� 
}

void Method_two (string &filename, vector<int> numbers, int split_count) { // ��k�G
	vector<vector<int>> numbers_splited = Split_nums(numbers, split_count);

	for (int i = 0; i < split_count; i++) { // ��C�Ӥ��μƦC��bubble sort 
		BubbleSort(numbers_splited[i]); // call Bubblesort
	}
	
	
	cout << "Merge";
	MergeSort(numbers_splited);
	cout << "Merge end";
	
	// �g�LMergeSort����Anumbers_splited�N�O���סA�N��g�J�ɮפ� 
	WriteFile(filename, numbers_splited, 2); // �ͦ��ɮ� 
}

int main() {
	string filename; // �ɦW
	int split_count; // ���Φ�split_count��
	int method_num; // �����method_num��k
	
	cout << "�п�J�ɮצW��:" << endl;
	cin >> filename;
	cout << "�п�J�n�����X��:" << endl;
	cin >> split_count;
	cout << "�п�J��k�s��:(��k1,��k2,��k3,��k4)" << endl;
	cin >> method_num;
	
	filename = filename + ".txt"; // �[�W���ɦW
	cout << "Ū�J��Ƶ���" << '\t'; 
	
	vector<int> num_list = Infile(filename);
	
	// �p��{���}�l���檺 CPU �ɶ�
    auto start_cpu = std::chrono::high_resolution_clock::now();
    
    // ����{��ɶ�
    auto current_time = chrono::system_clock::now();
    time_t current_time_t = chrono::system_clock::to_time_t(current_time);
    
    // �榡�Ʋ{��ɶ�
    tm time_info;
    localtime_r(&current_time_t, &time_info);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);
    auto now_ms = chrono::time_point_cast<chrono::milliseconds>(current_time);
    sprintf(buffer, "%s.%06d+08:00", buffer, static_cast<int>(now_ms.time_since_epoch().count() % 1000000));
	
	
	if (method_num == 1) {		
		Method_one(filename, num_list);
	}
	else if (method_num == 2) {
		Method_two(filename, num_list, split_count);
	}
	
	// �{���������檺CPU�ɶ�
    auto end_cpu = std::chrono::high_resolution_clock::now();
	
	// �p�����ɶ�
    chrono::duration<double> cpu_duration = end_cpu - start_cpu;
    auto cpu_seconds = std::chrono::duration_cast<std::chrono::seconds>(cpu_duration);
    auto cpu_fraction = cpu_duration - cpu_seconds;
    
    
    // �H���[���e���覡���}�ɮ�
    ofstream file(filename, ios::app);
    if (file.is_open()) {
    	// �g�J�ɶ���T
    	file << "CPU Time : " << cpu_seconds.count() << "." << std::fixed << std::setw(12) << std::setfill('0') << static_cast<long long>(cpu_fraction.count() * 1e12) << endl;
    	// ��X�{��ɶ�
    	file << "Output Time : " << buffer << endl;
    	// �������
    	file.close();
	}
       
    return 0;
}
