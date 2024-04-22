#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

vector<int> Infile(string filename) { // 讀入數字 
	vector<int> numbers; // 數列
	ifstream file(filename); // 開啟檔案
	
    if (file.is_open()) {
        string line;
        while (getline(file, line)) { // 逐行讀取檔案內容
            istringstream iss(line);
            int number;
            while (iss >> number) { // 從每行中讀取數字
                numbers.push_back(number); // 將數字放入動態陣列
            }
        }
        file.close(); // 關閉檔案
    }
    
    return numbers;
}

void Change_output_name(string &filename, int method_num) {
	// 找到"."的位置 
    size_t dotPos = filename.find_last_of('.');
    
    // 找到正確位置後插入"_output" 
    if (dotPos != string::npos) {
        filename.insert(dotPos, "_output");
        
        // 重新找到"."的位置 
    	size_t dotPos = filename.find_last_of('.');
        
        // 加入後綴
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

void WriteFile(string &filename, vector<vector<int>> num_list, int method_num) { // 將數字寫入檔案
    
	cout << "開始寫入";
	
	// 更改filename已達output需求 
	Change_output_name(filename, method_num);

	// 建立一文件，將數據寫入 filename 檔案中
    ofstream outfile(filename);

	int list_num = 0; // 第幾個list 
	int num_num = 0; // 第幾個num 
	int count = 0;

    // 檢查文件是否成功打開
    if (outfile.is_open()) {
        // 將數列中的每個向量逐行寫入文件
        for (const auto& vec : num_list) {
        	outfile << "Sort :" << '\n';
        	
            // 將向量中的每個數字逐行寫入文件
            for (int number : vec) {
            	while (list_num < num_list.size()) {
            		while (num_num < num_list[list_num].size()) {	
            		    outfile << num_list[list_num][num_num] << '\n';
            		    num_num = num_num + 1;
            		    
            		    count++;
            		}
            		
            		list_num++; // 往下一個list 
					num_num = 0; // num_num初始化 
				}
            }
        }
        // 關閉文件流
        outfile.close();
    }
	else {
        // 如果無法打開文件，顯示錯誤訊息
        cout << "FAIL" << endl;
    }
    
    cout << "寫入結束"; 
}

vector<vector<int>> Split_nums (vector<int> numbers, int split_count) { // 將數字分割成split_count份 
	// split_count是分割數量(等同說明裡的k) 
	
	vector<vector<int>> list_of_nums(split_count); // 放置被分割的數字 
	int split_time = 0; // 分割成k份 = 切 k-1 刀 
	int cur_num = 0; // 先前vector裡的第幾個數字 
	
	while (cur_num < numbers.size()) {
		list_of_nums[split_time].push_back(numbers[cur_num]);
		cur_num++;
			
		if (list_of_nums[split_time].size() > (cur_num + split_count - 1) / split_count) { // 若是成立表示一份分割完成
			// 最後一個數列允許有較多數字，否則會出現分割成k+1的情況 
			if (split_time != split_count - 1) {
				split_time++; // 下一次push_back就是第二個數列了 
			}
		}
	} // end while
	
	return list_of_nums; // 回傳檔案 
}

void BubbleSort(vector<int> &num_list) { // 泡沫排序 
	int n = num_list.size();
	
    	// 實做bubble sort 
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (num_list[j] > num_list[j+1]) {
               	// 交換數字 
               	int temp = num_list[j];
               	num_list[j] = num_list[j+1];
               	num_list[j+1] = temp;
            }
        }
    }
}

void MergeSort(vector<vector<int>> &numbers);

void Extra_Merge(vector<vector<int>> &all_sorted_list, vector<int> right_vec) { // 額外的合併 
	// 遇到奇數個數列時所進行的額外一次合併
	// 方法:直接拿all_sorted_list的最後一個與多出的進行額外的合併 
	// 理論上應當也會是該合併循環的最後一次合併
	
	vector<int> sorted_list; // 額外的合併產生的數列，最後要取代all_sorted_list的最後一個數列 
	
	int i = 0, j = 0; // i 和 j 為陣列裡的第?個數字 
	
	// all_sorted_list[all_sorted_list.size() - 1] 指的是 all_sorted_list裡最後一個數列 
	 
	while(i < all_sorted_list[all_sorted_list.size() - 1].size() || j < right_vec.size()) {
		// 對陣列裡的數字做比較，較小的push_back進sorted_list 
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
	// 這邊不呼叫Merge，而是return回到上一動的位置，也就是呼叫MergeSort 
}

void Merge(vector<vector<int>> &all_sorted_list, // 每一個左右兩陣列合併後的陣列組成的陣列 
	vector<vector<int>> numbers, int left_num, int right_num) { // 合併
	
	// number = 欲合併的數列
	// all_sorted_list = 合併好的數列(所以function一開始應當為空vector) 
	
	vector<int> left_vec = numbers[left_num]; // 決定左陣列 
	vector<int> right_vec = numbers[right_num]; // 決定右陣列 

	vector<int> sorted_list; // 左右兩陣列的合併的陣列 
	
	int i = 0, j = 0; // i 和 j 為陣列裡的第?個數字 
	while(i < left_vec.size() || j < right_vec.size()) {
		// 對陣列裡的數字做比較，較小的push_back進sorted_list 
		if (left_vec[i] < right_vec[j]) { 
			sorted_list.push_back(left_vec[i]);
			i++;
		}
		else if (left_vec[i] > right_vec[j]) {
			sorted_list.push_back(right_vec[j]);
			j++;
		}
	}
	
	all_sorted_list.push_back(sorted_list); // 將排列好的 子陣列:sorted_list 放入 父陣列:all_sorted_list
	
	left_num = left_num + 1;
	right_num = right_num - 1;
	
	cout << left_num << " / " << right_num << endl;
	
	vector<vector<int>> blank_list; // 空vector，在呼叫時傳入一空vector和排列好的vector 
	
	if (left_num == right_num) { // 奇數個數列，有人落單，安排他跟上一個sorted_list額外進行一次sort 
		Extra_Merge(all_sorted_list, numbers[right_num]);
	}
	if (left_num < right_num) {
		Merge(blank_list, all_sorted_list, left_num, right_num); // 這一輪尚未排列完整，繼續進行 
	}
	
	cout << all_sorted_list.size();
	MergeSort(all_sorted_list); // 全部排列完一輪，進行下一次的排列 
}

void MergeSort(vector<vector<int>> &numbers) { // 合併排序 
	if (numbers.size() <= 1) { // 全部的陣列已合併成一個經過排序的陣列 
		return;
	}
	
	vector<vector<int>> result_list; // 排序完的陣列 
	
	int left_num = 0;
	int right_num = numbers.size() - 1; // 由0開始 
	
	Merge(result_list, numbers, left_num, right_num);
	
	numbers = result_list;
}

/*
    // 記錄程式開始執行的時間點
    auto start = std::chrono::high_resolution_clock::now();

    // 在這裡放入你的程式碼

    // 記錄程式執行完畢的時間點
    auto end = std::chrono::high_resolution_clock::now();

    // 計算程式執行的時間差
    std::chrono::duration<double> duration = end - start;

    // 輸出執行時間
    std::cout << "程式執行時間: " << duration.count() << " 秒\n";
*/ 

void Method_one (string &filename, vector<int> numbers) { // 方法一 
    BubbleSort(numbers); // call Bubblesort
    
    // 由於WriteFile要求 vector<vector<int>> 檔案，所以新增一個
	
	vector<vector<int>> sorted_number;
	sorted_number.push_back(numbers);
    
    WriteFile(filename, sorted_number, 1); // 生成檔案 
}

void Method_two (string &filename, vector<int> numbers, int split_count) { // 方法二
	vector<vector<int>> numbers_splited = Split_nums(numbers, split_count);

	for (int i = 0; i < split_count; i++) { // 對每個分割數列做bubble sort 
		BubbleSort(numbers_splited[i]); // call Bubblesort
	}
	
	
	cout << "Merge";
	MergeSort(numbers_splited);
	cout << "Merge end";
	
	// 經過MergeSort之後，numbers_splited就是答案，將其寫入檔案中 
	WriteFile(filename, numbers_splited, 2); // 生成檔案 
}

int main() {
	string filename; // 檔名
	int split_count; // 分割成split_count份
	int method_num; // 執行第method_num方法
	
	cout << "請輸入檔案名稱:" << endl;
	cin >> filename;
	cout << "請輸入要切成幾份:" << endl;
	cin >> split_count;
	cout << "請輸入方法編號:(方法1,方法2,方法3,方法4)" << endl;
	cin >> method_num;
	
	filename = filename + ".txt"; // 加上副檔名
	cout << "讀入資料結束" << '\t'; 
	
	vector<int> num_list = Infile(filename);
	
	// 計算程式開始執行的 CPU 時間
    auto start_cpu = std::chrono::high_resolution_clock::now();
    
    // 獲取現實時間
    auto current_time = chrono::system_clock::now();
    time_t current_time_t = chrono::system_clock::to_time_t(current_time);
    
    // 格式化現實時間
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
	
	// 程式結束執行的CPU時間
    auto end_cpu = std::chrono::high_resolution_clock::now();
	
	// 計算執行時間
    chrono::duration<double> cpu_duration = end_cpu - start_cpu;
    auto cpu_seconds = std::chrono::duration_cast<std::chrono::seconds>(cpu_duration);
    auto cpu_fraction = cpu_duration - cpu_seconds;
    
    
    // 以附加內容的方式打開檔案
    ofstream file(filename, ios::app);
    if (file.is_open()) {
    	// 寫入時間資訊
    	file << "CPU Time : " << cpu_seconds.count() << "." << std::fixed << std::setw(12) << std::setfill('0') << static_cast<long long>(cpu_fraction.count() * 1e12) << endl;
    	// 輸出現實時間
    	file << "Output Time : " << buffer << endl;
    	// 關閉文件
    	file.close();
	}
       
    return 0;
}
