

Cấu trúc gần giống mô hình MVC, để hiểu rõ hơn có thể xem qua class diagram Overview.cd


		Class giữ dữ liệu là TilesMap.
		Trong TilesMap có các thành phần thuộc lớp khác để hỗ trợ việc save và load dữ liệu bằng file xml.
		TilesMap chứa một đối tượng TilesSet, đối tượng tileset chứa thông tin về ảnh tile, số dòng số cột, độ rộng, độ cao của tile, danh sách các tile
		Mỗi Tile chứa Id, srcRect, reference đến anh của Tileset.
			srcRect để vẽ ra map.
			id để so sánh với ma trận index của tilemap.

		MapController là lớp trung gian giao tiếp giữa form và các lớp chứa dữ liệu.

		MainForm chứa các chức năng:
			NewMap: tạo ra một đối tượng MapController mới, đối tượng này sẽ tạo ra các đối tượng dữ liệu liên quan.
			CreateTiles: tạo ra một danh sách các tile từ một ảnh, dữ liệu được lưu trữ trong đối tượng lớp TileSet.
			CloneMap: sao chép ma trận index từ một map hoàn chỉnh.
			Save: lưu map lại dưới dạng file xml, kèm theo một file ảnh, hai file này sẽ được lưu cùng thư mục, nếu thiếu file ảnh sẽ phát sinh lỗi
			Load: load map từ file xml.


Cấu trúc file xml:
	
		<Tilesmap columns="5" rows="5">					//columns rows là số tile theo chiều ngang, cao của map
			<MatrixIndex>								// ma trận index dùng để so sánh với id của Tile
				<Row id="0">0	4	0	1	7</Row>		
				<Row id="1">6	8	2	3	12</Row>
				....
			</MatrixIndex>
			<TileSet Columns="10" Rows="12" Widthtile="32" Heighttile="32" FileName="stage1.png">				// Số dòng, số cột, rộng, cao của tile.
				<Tiles>																	//
					<Tile Id="1" Name="tile_1">											// Các tile trong listtile, chứa id, tên sẽ là tên hiển thị trong listview
						<Rect X="0" Y="0" Width="31" Height="31" />						// soureRect.
					</Tile>
					<Tile Id="2" Name="tile_2">
						<Rect X="32" Y="0" Width="31" Height="31" />
					</Tile>
					....
				</Tiles>
			</TileSet>
			[some other object]
		</Tilesmap>

Trong cấu trúc xml chúng ta luôn cần một root. Root của chúng ta là Tilesmap.

Vậy nếu muốn thêm đối tượng nào vào, thì cách đơn giản nhất là, ta cho đối tượng ấy trở thành một thuộc tính của lớp TilesMap.
Trong cấu trúc xml thì thêm các element mới vào [some other object].
Các kiểu dữ liệu cơ sở nên lưu ở attribute.
Các kiểu dữ liệu tự định nghĩa lưu thành element con.

Có thể xem ví dụ file stage1.xml trong thư mục Wingame\Resources\Map