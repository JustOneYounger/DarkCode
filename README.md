# DarkCode
自制Python编辑器

Qt版本:6.5.3

内存在诸多BUG，尚未修复，还请见谅

## Source文件夹
Source文件夹内为源码，未编译构建

## DarkCode.exe
DarkCode.exe为Qt Creator内构建形成的可执行文件，需要自行使用windeployqt进行打包

## 备注
程序执行会在当前文件夹形成一个按照当前时间戳命名的python文件，用于代码的执行，本意是想设计一个选中部分python也能执行的效果，但是执行全部python，我也将它暂存到了这个时间戳命名的文件，所以import当前项目文件夹内文件以及程序中使用打开文件等功能时需要填写绝对地址，否则会运行报错，这个我会慢慢修改的
