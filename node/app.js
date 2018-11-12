var httpserver = require("http");
var qs = require("querystring");
var url = require("url");
var fs = require("fs");

httpserver.createServer(onRequest).listen(3001);

function onRequest(request,response)
{
    var pathname = url.parse(request.url).pathname;
    if(pathname=="/")    //访问表单页面
    {
        response.writeHead(200,{"Content-Type":"text/html"});
        fs.readFile("index.html","utf-8",function(e,data){
            response.write(data);
            response.end();
        });
    }
    else if(pathname=="/postlogin")    //处理post方式请求
    {
        var urlstr="";
        var jsondata;
        request.addListener("data",function(postdata){
            urlstr+=postdata;    //接收到的表单数据字符串，这里可以用两种方法将UTF-8编码转换为中文
            jsondata = qs.parse(urlstr);        //转换成json对象
            var decodedata = decodeURIComponent(urlstr);        //对表单数据进行解码
            console.log(urlstr);
            console.log(jsondata);
            console.log(decodedata);
            urlstr = decodedata;
        });
        request.addListener("end",function(){
            response.writeHead(200,{"Content-Type":"text/plain; charset=utf-8"});
            response.write(urlstr);
            response.end();
        });
    }
    else if(pathname=="/getlogin")    //处理get方式请求
    {
        var urlstr = url.parse(request.url).query;
        var jsondata = qs.parse(urlstr);
        var decodedata = decodeURIComponent(urlstr);

        console.log(urlstr);
        console.log(jsondata);
        console.log(decodedata);
        urlstr = decodedata;

        response.writeHead(200,{"Content-Type":"text/plain; charset=utf-8"});
        response.write(urlstr);
        response.end();
    }
    else
    {
        response.writeHead(200,{"Content-Type":"text/plain"});
        response.write("error");
        response.end();
    }
}