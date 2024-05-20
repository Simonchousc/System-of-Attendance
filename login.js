// 生成并显示验证码
var captcha;
function generateCaptcha() {
    const canvas = document.getElementById('captchaCanvas');
    const ctx = canvas.getContext('2d');
    const chars = 'ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnpqrstuvwxyz123456789';
    captcha = "";

    // 生成五字符的验证码
    for (let i = 0; i < 5; i++) {
        captcha += chars.charAt(Math.floor(Math.random() * chars.length));
    }

    // 在显示新的验证码之前清空画布
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.font = '30px Arial';
    ctx.fillStyle = '#007BFF';

    // 随机字符验证码
    for (let i = 0; i < captcha.length; i++) {
        const x = 10 + i * 18;
        const y = 30 + Math.random() * 5;
        const angle = Math.random() * 0.3 - 0.15;
        ctx.save();
        ctx.translate(x, y);
        ctx.rotate(angle);
        ctx.fillText(captcha.charAt(i), 0, 0);
        ctx.restore();
    }

    // 划线
    for (let i = 0; i < 5; i++) {
        ctx.beginPath();
        ctx.moveTo(Math.random() * canvas.width, Math.random() * canvas.height);
        ctx.lineTo(Math.random() * canvas.width, Math.random() * canvas.height);
        ctx.strokeStyle = '#007BFF';
        ctx.stroke();
    }
}

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



function login_in() {
    //alert("000");
    // 获取页面输入数据
    var captcha_in = document.getElementById("captcha_id").value;

    var pwd = document.getElementById("pwd").value;
    var usr = document.getElementById("usr").value;
    var loginType = document.getElementById('loginType').value;
    // 创建 AJAX 对象
    var xmlhttp = getXMLHttpRequest();
    // 编写回调函数
    xmlhttp.onreadystatechange = function () {
        // 浏览器发送请求成功并且服务器正确响应
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            var response = JSON.parse(xmlhttp.responseText);
            //captcha == captcha_in && 
            if (captcha === captcha_in && response.success) {

                if (loginType == "admin") {

                    window.location.href = "./admin.html";  //跳转admin页面  
                } else {
                    window.location.href = "./user.html";  //跳转user页面
                    // 用户登录成功后保存用户名和密码到 Local Storage
                    localStorage.setItem('usr', usr);
                    localStorage.setItem('pwd', pwd);
                    //console.log(usr + pwd);
                }

            } else {
                alert("登录失败!");
                generateCaptcha(); // 失败时刷新验证码
            }
        }
    };

    var data = {
        username: usr,
        password: pwd,
        type: loginType
    };
    var url = "/cgi-bin/login.cgi";
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-Type", "application/json");
    xmlhttp.send(JSON.stringify(data));
}


// 页面加载时初始化验证码
window.onload = generateCaptcha;