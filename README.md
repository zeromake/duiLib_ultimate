### **Duilib_Ultimate**

*** 
#### **目录**
* [项目简介](#项目简介)
* [功能简介](#功能简介)
* [作者简介](#作者简介)
* [项目支持](#项目支持)

***
#### **项目简介** 

DuiLib_Ultimate是duilib库的增强拓展版，库修复了大量用户在使用中反馈的Bug，新增了用户需求的功能，并持续维护更新。
更多效果例子请查看：https://github.com/qdtroy/DuiLib_Demos

#### **功能简介** 

1、官方原有功能；

2、增加资源管理器支持，资源管理更加方便；

3、支持多国语言功能；

4、高DPI自适应的机制（接口简单，实用方便）；

6、控件支持样式表（Style）功能，控件属性管理设置更加方便易用；

7、新增功能（DateTime控件、IP地址控件、颜色选择控件、List网格线、List多选等）；

8、修正大量Bug：RichEdit输入变化消息、Combo字体设置、Button自动布局、控件右下边框绘制不完全、菜单控件崩溃等；

9、项目内置各种使用例子与编译选项（多字节Unicode，静态库和动态库）

#### **作者简介** 
网名：Troy，QQ：656067418，微信：qdtroy

duilib开源项目圈（QQ群）：1群（261675375）、2群（261851826）、3群（199950533）

##### **项目支持** 
学习指导、技术支持请联系Troy（QQ：656067418）


## zeromake 对该项目修改

- [x] 编译工具迁移至 `xmake` 比起 `vs` 的编译管理真的是太好用了。
- [x] 代码里的 `lib` 加载声明，全部迁移到 `xmake` 项目描述里。
- [x] `xml` 资源文件加载现在可以自动识别 `bom` 头，无法识别的强制为 `utf-8`，xml 现在可以 `utf-8`、`utf16le`、`utf16be` 混用。
- [x] 修复在非 `unicode` 模式下，`DrawTextA` 渲染 `utf-8` 文字乱码(使用 DrawTextW 替代)。
- [x] 修复在非 `unicode` 模式下的 `utf16le` 的 `xml` 文件无法正常解析(CharNext使用的编码不正确)。
- [x] 封装 `cximage` 到单独的目标编译，与 `duilib` 分离。
- [x] 把 `ActiveX` 相关组件提取到单独的目标编译，与 `duilib` 分离。
- [x] 迁移代码以支持 `mingw64` 进行编译，主要是一些 `atl`, `com` 的引用问题，还有 `com` 组件的 `GUID` 声明在 `mingw` 里没有(现在是手动补了)。
- [x] 修复在 `mingw` 下的 `dpi` 窗口初始化会出现窗口异常，`CPaintManagerUI::SetDPI` 里的 `RECT*` 指针引用会乱掉，现在直接用 `struct` 了。
- [x] 在加载 `xml` 布局前就会初始化 `dpi`，现在可以在 `exe` 的 `manifest` 声明 `dpi` 感知，也可以手动调用 `CPaintManagerUI::InitProcessDPIAware();` 来设置 `dpi` 感知，然后除了 `dpi` 变化事件就没有了。
- [ ] 好多 `demo` 还没有迁移过来测试。
- [ ] 参考 `android` 的 `ListView` 测试。
- [ ] 控件拆出去，保持 `duilib` 核心代码精简化。
- [ ] 参考 `krkr2` 的插件加载方式设计插件化加载。
- [ ] xml 文件加载解析，使用第三方库替代。
- [ ] 事件注册参考 `html` + `js`。
- [ ] `svg` 支持。
