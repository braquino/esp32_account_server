#include "html.hpp"

Html::Html() {}

String get_req_header(bool ok = true) {
  return "HTTP/1.1 200 OK\nContent-type:text/html\nConnection: close\n\n";
}

String tag(const String& html_peace, String t) {
  return "<" + t + ">" + html_peace + "</" + t + ">";
}

String Html::get_insert_page() {
  return get_req_header() + R"--(<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {
      font-family: Helvetica;
      color: white
    }
    .form {
      padding: 3px 40px;
      font-size: 14;
    }
    .button {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 3px 40px;
      text-decoration: none;
      font-size: 20px;
      margin: 2px;
      cursor: pointer;
    }
    .button2 {
      background-color: #555555;
    }
  </style>
</head>
<body style="background-color: #555555;color: #ffffff;">
  <table>
    <tr><h1>Insert an expense:</h1></tr>
    <tr><form name="expense" action="/records_page.html" class="form">
      <table>
        <tr>
          <td>
            <label for="fname">Day (yyyy-mm-dd):</label>
          </td>
          <td>
            <input type="text" id="date" name="date" required pattern="^\d{4}-([0]\d|1[0-2])-([0-2]\d|3[01])$">
          </td>
        </tr>
        <tr>
          <td>
            <label for="fname">Description:</label>
          </td>
          <td>
            <input type="text" id="desc" name="desc" required>
          </td>
        </tr>
        <tr>
          <td>
            <label for="fname">Value (0.00):</label>
          </td>
          <td>
            <input type="text" id="value" name="value" required pattern="^[-]?\d*(\.\d*)?$">
          </td>
        </tr>
      </table>
      <p>
      <input type="submit" value="submit" class="button">
    </p>
    </form>
  </tr>
  </table>
</body>
<script>
  var todayDate = new Date().toISOString().slice(0, 10);
  document.forms["expense"]["date"].value = todayDate;
</script>
</html>

)--";
}

String Html::get_records_page(const std::vector<std::tuple<long, String, String, float>>& records) {
  String html_rows;
  float sum = 0.0;
  for (const std::tuple<long, String, String, float>& t : records) {
    float v = std::get<3>(t);
    long id = std::get<0>(t);
    sum += v;
    html_rows += "<tr><td>" + String(id) + "</td><td>" + std::get<1>(t) + "</td><td>" + std::get<2>(t) + "</td><td>" + String(v) + "</td><tr>";
  }
  html_rows += "<tr><th></th><th></th><th>Total</th><th id=\"total\">" + String(sum) + "</th><tr>";
  return R"--(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {
      font-family: Helvetica;
      color: white
    }
    .form {
      padding: 3px 40px;
      font-size: 14;
    }
    .button {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 3px 40px;
      text-decoration: none;
      font-size: 20px;
      margin: 2px;
      cursor: pointer;
    }
    .button2 {
      background-color: #555555;
    }
    table.exp {
      max-width: 400px;
      width: 100%;
    }
    table.exp th, table.exp td {
      border: 1px solid white;
      border-collapse: collapse;
    }
    table.exp th {
      background-color: #333333;
      font-style: bold;
    }
    table.exp td {
      background-color: #777777;
      text-align: left;
    }
  </style>
</head>
<body style="background-color: #555555;color: #ffffff;">
  <table>
    <tr>
      <h1>Expense list:</h1>
    </tr>
    <tr>
      <table class="exp">
        <tr>
          <th style="width: 10%;">Id</th>
          <th style="width: 30%;">Date</th>
          <th style="width: 40%;">Description</th>
          <th style="width: 30%;">Value</th>
        </tr>)--" + html_rows +

R"--(      <tr><th></th><th></th><th id="sit_label"></th><th id="situation"></th><tr></tr>
      </table>
      <p>
        <a href="/insert_page.html"><button class="button" style="width: 255px;">Insert expense</button></a>
      </p>
      <p>
        <form name="delete" action="/records_page.html">
          <table>
            <tr>
              <td>
                <label for="fname">Id:</label>
              </td>
              <td>
                <input type="text" id="id" name="id" required pattern="^\d*$" style="width: 50px;">
              </td>
              <td>
                <input type="submit" value="Del record" class="button">
              </td>
        </form>
      </p>
    </tr>
  </table>
</body>
<script>
  var currentDay = new Date().getDate();
  var total = document.getElementById("total");
  var sit_label = document.getElementById("sit_label");
  sit_label.innerText = "Situation on day: " + currentDay;
  var situaltion = document.getElementById("situation");
  situaltion.innerText = (30 * currentDay) - Number(total.innerText);
</script>
</html>

)--";
}