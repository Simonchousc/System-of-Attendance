
function getXMLHttpRequest() {
    var xmlhttp = null;
    if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();//创建请求对象
    }
    else////如果浏览器是低版本的
    {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
    }
    return xmlhttp;//返回请求对象
}


var newIc = "";
//获取新卡号
function getNewCard() {
    var xmlhttp = getXMLHttpRequest(); // 使用封装的函数获取 XMLHttpRequest 对象

    // 设置请求完成时的回调函数
    xmlhttp.onreadystatechange = function () {

        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            var data = JSON.parse(xmlhttp.responseText); // 解析服务器返回的JSON数据
            newIc = data.data[0][0]; // 获取 data 中的 ic 字符串
            //console.log("ic" + newIc);
        }

    };

    // 发送GET请求到服务器
    xmlhttp.open("GET", "/cgi-bin/getnewcard.cgi", true);
    xmlhttp.send();
}



// 注册新用户
function registerUser() {
    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    const realName = document.getElementById('realName').value;
    const gender = document.getElementById('gender').value;
    const icCard = newIc;



    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange = function () {

        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
                alert("用户注册成功！");
                // 清空输入框
                document.getElementById('username').value = '';
                document.getElementById('password').value = '';
                document.getElementById('realName').value = '';
                document.getElementById('gender').value = '';
                document.getElementById('icCard').value = '';
            } else {
                alert("用户注册失败：" + xmlhttp.responseText);
            }
        }
    };


    // 组装要发送的数据对象
    const userData = {
        username: username,
        password: password,
        realName: realName,
        gender: gender,
        icCard: icCard
    };

    // 发送POST请求到服务器，
    xmlhttp.open("POST", "/cgi-bin/register.cgi", true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(userData)); // 发送 JSON 格式的数据

}



function fetchData() {
    var xmlhttp = getXMLHttpRequest(); // 使用封装的函数获取 XMLHttpRequest 对象

    // 设置请求完成时的回调函数
    xmlhttp.onreadystatechange = function () {

        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            var data = JSON.parse(xmlhttp.responseText); // 解析服务器返回的JSON数据
            //console.log("Parsed data: ", data); // 打印解析后的数据
            displayData(data); // 显示数据
        }

    };

    // 发送GET请求到服务器
    xmlhttp.open("GET", "/cgi-bin/main.cgi", true);
    xmlhttp.send();
}

function fetchData_user() {
    var xmlhttp = getXMLHttpRequest(); // 使用封装的函数获取 XMLHttpRequest 对象

    // 设置请求完成时的回调函数
    xmlhttp.onreadystatechange = function () {

        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            var data = JSON.parse(xmlhttp.responseText); // 解析服务器返回的JSON数据
            //console.log("Parsed data: ", data); // 打印解析后的数据
            displayData_user(data); // 显示数据
        }

    };

    // 发送GET请求到服务器
    xmlhttp.open("GET", "/cgi-bin/user_show.cgi", true);
    xmlhttp.send();
}

function displayData_user(data) {
    var recordsTableBody = document.getElementById("usersTableBody");
    recordsTableBody.innerHTML = ""; // 清空表格内容

    // 判断数据是否有效
    if (data && data.rows && data.cols && data.data) {
        for (var i = 0; i < data.rows; i++) {
            var row = recordsTableBody.insertRow(); // 插入新行

            for (var j = 0; j < data.cols; j++) {
                var cell = row.insertCell(); // 插入新单元格
                cell.textContent = data.data[i][j]; // 设置单元格内容为数据项的值
            }

            // 在每行最后一个单元格插入删除按钮
            var deleteButtonCell = row.insertCell(); // 插入新单元格
            var deleteButton = document.createElement("button"); // 创建删除按钮
            deleteButton.textContent = "删除";
            deleteButton.setAttribute("onclick", "deleteUser('" + data.data[i][0] + "')"); // 设置按钮点击事件
            deleteButtonCell.appendChild(deleteButton); // 将按钮添加到单元格中
        }
    } else {
        console.error("Invalid data format:", data);
    }
}

// 删除用户
function deleteUser(icCard) {
    var xmlhttp = getXMLHttpRequest();

    // 设置请求完成时的回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
                // 删除用户成功，刷新用户列表
                fetchData_user();
            } else {
                alert(xmlhttp.status);
                alert("删除用户失败，请重试。");
            }
        }
    };

    // 构建要发送的 JSON 数据
    var requestData = {
        icCard: icCard
    };

    // 发送POST请求到服务器，以删除指定的用户
    xmlhttp.open("POST", "/cgi-bin/delete_user.cgi", true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(requestData)); // 发送 JSON 格式的数据
}


// 将考勤数据显示在页面上
function displayData(data) {
    var recordsTableBody = document.getElementById("recordsTableBody");
    recordsTableBody.innerHTML = ""; // 清空表格内容

    // 判断数据是否有效
    if (data && data.rows && data.cols && data.data) {
        for (var i = 0; i < data.rows; i++) {
            var row = recordsTableBody.insertRow(); // 插入新行

            for (var j = 0; j < data.cols; j++) {
                var cell = row.insertCell(); // 插入新单元格
                cell.textContent = data.data[i][j]; // 设置单元格内容为数据项的值
            }
        }
    } else {
        console.error("Invalid data format:", data);
    }
}


// 切换显示的部分
function showSection(sectionId) {

    const sections = document.querySelectorAll('.section');
    sections.forEach(section => {
        section.classList.remove('active');
    });
    document.getElementById(sectionId).classList.add('active');

}

function registeric() {
    document.getElementById('icCard').value = newIc;
}
// 页面加载完成时执行 fetchData 函数
window.onload = function () {
    // fetchData();
    // fetchData_user();
    // getNewCard();
    // registeric();
    setInterval(fetchData, 5000);
    setInterval(fetchData_user, 5000);
    setInterval(registeric, 3000);
    setInterval(getNewCard, 3000);
};



