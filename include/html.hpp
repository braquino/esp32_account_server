#ifndef HTML_HPP_
#define HTML_HPP_

#include <WString.h>
#include <tuple>
#include <vector>

class Html {
public:
    Html();
    String get_home(bool btn_5_press, bool btn_2_press);
    String get_insert_page();
    String get_records_page(const std::vector<std::tuple<long, String, String, float>>& records);
private:
    
};

#endif // HTML_HPP_