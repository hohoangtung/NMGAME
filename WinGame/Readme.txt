

Convention:
	Tên hàm viết thường chữ cái đầu, viết hoa các chữ cái đầu từ tiếp theo
	Tên biến thành viên thêm "_" ở đầu, viết hoa các chữ cái đầu từ tiếp theo
	Tên biến cục bộ, tên tham số viết thường chữ cái đầu
	Tên enum thêm e đầu tên, ví dụ eStatus, định nghĩa bên trong biết hoa tất cả chữ cái
	Tên kiểu dữ liệu được định nghĩa lại thêm G đầu từ, nếu là con trỏ thì thêm p
		ví dụ typedef D3DXVECTOR2 GVector2;

	định nghĩa các thành phần toàn cục trong file define.h

Khi commit project không commit các file vcxproj, file sdf, các file trong thư mục debug, release.
Khi update project nhớ kiểm tra các file được load về, nếu có file mới cần add vào project, với filter giống với thư mục