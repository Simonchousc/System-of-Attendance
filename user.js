


/**************************************************************************************************** */
function getXMLHttpRequest() {
    var xmlhttp = null;
    if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();//创建请求对象
    }
    else////如果浏览器是底版本的
    {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
    }
    return xmlhttp;//返回请求对象
}


// 发送GET请求获取个人信息
function fetchUserInfo() {
    // 从 Local Storage 中获取保存的用户名和密码
    const username = localStorage.getItem('usr');
    const password = localStorage.getItem('pwd');
    //alert(username + password);
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) { // 请求成功
                var data = JSON.parse(xmlhttp.responseText);
                //console.log(data);
                displayUserInfo(data);
            } else {
                console.error('Failed to fetch user info:', xmlhttp.statusText);
            }
        }
    };

    var postdata = {
        username: username,
        password: password
    }
    xmlhttp.open("POST", "/cgi-bin/get_user_info.cgi", true); // 发送GET请求到服务器
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(postdata)); // 发送 JSON 格式的数据
}

// 展示当前用户信息
function displayUserInfo(data) {
    if (data.data && data.data.length >= 1 && data.data[0].length >= 5) {
        var userData = data.data[0]; // 获取第一个数组元素，即用户信息数组

        // 填充输入框
        document.getElementById('icCard').value = userData[0]; // IC 卡号
        document.getElementById('username').value = userData[1]; // 用户名
        document.getElementById('password').value = userData[2]; // 密码
        document.getElementById('realName').value = userData[3]; // 真实姓名
        document.getElementById('gender').value = userData[4]; // 性别

    } else {
        console.error('Invalid user data:', data);
    }
}

// 显示相应的部分
function showSection(sectionId) {
    // 隐藏所有部分
    var sections = document.querySelectorAll('.section');
    sections.forEach(function (section) {
        section.classList.remove('active');
    });
    // 显示指定部分
    var selectedSection = document.getElementById(sectionId);
    selectedSection.classList.add('active');
}


// 更新个人信息
function updateProfile() {
    var xmlhttp = getXMLHttpRequest();

    const username = document.getElementById('username').value;
    const password = document.getElementById('password').value;
    const realName = document.getElementById('realName').value;
    const gender = document.getElementById('gender').value;
    const icCard = document.getElementById('icCard').value;

    // 设置请求完成时的回调函数
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) {
                // 更新成功

                fetchUserInfo();
                localStorage.setItem('usr', username);
                localStorage.setItem('pwd', password);
                alert("更新成功");
            } else {
                alert(xmlhttp.status);
                alert("更新失败，请重试。");
            }
        }
    };

    // 构建要发送的 JSON 数据
    var requestData = {
        icCard: icCard,
        username: username,
        password: password,
        realName: realName,
        gender: gender
    };

    // 发送POST请求到服务器，更新用户信息
    xmlhttp.open("POST", "/cgi-bin/update_user.cgi", true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(requestData)); // 发送 JSON 格式的数据
}


// 发送POST请求获取用户的考勤记录
function fetchAttendanceRecords() {
    // 从 Local Storage 中获取保存的用户名和密码
    const username = localStorage.getItem('usr');
    const password = localStorage.getItem('pwd');
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange = function () {
        if (xmlhttp.readyState == 4) {
            if (xmlhttp.status == 200) { // 请求成功
                var data = JSON.parse(xmlhttp.responseText);
                updateAttendanceTable(data);
            } else {
                console.error('获取用户考勤信息失败', xmlhttp.statusText);
            }
        }
    };

    var postdata = {
        username: username,
        password: password
    }
    xmlhttp.open("POST", "/cgi-bin/user_records.cgi", true); // 发送POST请求到服务器
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(postdata)); // 发送 JSON 格式的数据
}

// 将考勤数据显示在页面上
function updateAttendanceTable(data) {
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






// 在页面加载时调用函数
window.onload = function () {
    fetchUserInfo();
    fetchAttendanceRecords();
    setInterval(fetchAttendanceRecords, 3000);
};
