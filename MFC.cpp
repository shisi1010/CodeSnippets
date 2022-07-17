/*************************************************
函数名:    	ShowImage
函数描述:	在 MFC Picture Control 控件上显示opencv图像（控件指定为IDC_SHOW）
输入参数:   imgSrc-待显示的图像(OpenCV库Mat对象结构)
输出参数:
返回值:
**************************************************/
void CDLG::ShowImage(cv::Mat imgSrc)
{
    // 获取图片控件矩形框
    CRect rect;
    GetDlgItem(IDC_CAMERA)->GetClientRect(&rect);

    //缩放Mat，以适应图片控件大小
    cv::resize(imgSrc, imgSrc, cv::Size(rect.Width(), rect.Height()));

    // 转换格式 ,便于获取BITMAPINFO
    switch (imgSrc.channels())
    {
    case 1:
        cv::cvtColor(imgSrc, imgSrc, cv::COLOR_GRAY2BGRA); // GRAY单通道
        break;
    case 3:
        cv::cvtColor(imgSrc, imgSrc, cv::COLOR_BGR2BGRA); // BGR三通道
        break;
    default:
        break;
    }

    // 计算一个像素多少个字节
    int pixelBytes = imgSrc.channels() * (imgSrc.depth() + 1);

    // 制作bitmapinfo(数据头)
    BITMAPINFO bitInfo;
    bitInfo.bmiHeader.biBitCount = 8 * pixelBytes;
    bitInfo.bmiHeader.biWidth = imgSrc.cols;
    bitInfo.bmiHeader.biHeight = -imgSrc.rows; //注意"-"号(正数时倒着绘制)
    bitInfo.bmiHeader.biPlanes = 1;
    bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitInfo.bmiHeader.biCompression = BI_RGB;
    bitInfo.bmiHeader.biClrImportant = 0;
    bitInfo.bmiHeader.biClrUsed = 0;
    bitInfo.bmiHeader.biSizeImage = 0;
    bitInfo.bmiHeader.biXPelsPerMeter = 0;
    bitInfo.bmiHeader.biYPelsPerMeter = 0;

    CDC *pDC = GetDlgItem(IDC_CAMERA)->GetDC(); //获取图片控件DC

    //绘图
    ::StretchDIBits(
        pDC->GetSafeHdc(),
        0, 0, rect.Width(), rect.Height(),
        0, 0, imgSrc.cols, imgSrc.rows,
        imgSrc.data,
        &bitInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
    ReleaseDC(pDC); //释放DC
}
