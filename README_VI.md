---
date: 2026-09-08
description: "README tiếng Việt cho MacEverything"
tags: [maceverything, docs]
---

<p align="center">
  <img src="MacEverything/Assets.xcassets/AppIcon.appiconset/icon_256.png" alt="MacEverything" width="128" />
</p>

<h1 align="center">MacEverything</h1>

<p align="center">
  <b>Công cụ tìm file siêu tốc cho macOS</b> — định vị bất kỳ file nào trong hàng triệu file chỉ trong mili giây.<br/>
  Lấy cảm hứng từ <a href="https://www.voidtools.com/">Everything</a> trên Windows, trên Mac không có đối thủ.
</p>

<p align="center">
  <a href="README.md">中文</a> | <a href="README_EN.md">English</a> | <b>Tiếng Việt</b>
</p>

<p align="center">
  <a href="#cài-đặt"><img src="https://img.shields.io/badge/macOS-13%2B-blue?logo=apple" alt="macOS 13+" /></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/license-MIT-green" alt="MIT License" /></a>
  <a href="#hệ-thống-test"><img src="https://img.shields.io/badge/tests-81%20modules-brightgreen" alt="81 test modules" /></a>
  <a href="#tích-hợp-ai-mcp"><img src="https://img.shields.io/badge/MCP-compatible-blueviolet" alt="MCP Compatible" /></a>
</p>

---

<p align="center">
  <img src="assets/screen-shot.jpg" alt="MacEverything Screenshot" width="720" />
</p>

## Điểm nổi bật

### Tìm kiếm cực nhanh

Index toàn bộ ổ đĩa **5 triệu+ file chỉ trong ~14 giây**, sau đó mỗi lần tìm **dưới 5ms** đã có kết quả. Nhanh hơn Spotlight hai bậc độ lớn.

| Tiêu chí | MacEverything | Spotlight | `find` |
|--------|:---:|:---:|:---:|
| Index 5 triệu file | ~14 giây | Vài phút trở lên | Không index |
| Độ trễ tìm kiếm | **< 5ms** | 200ms–2s | 5–30s |
| Theo dõi realtime | FSEvents | FSEvents | Không |
| Tìm nội dung | Index Trigram | Chủ yếu metadata | `grep` |
| Tích hợp AI | MCP built-in | Không | Không |

### Gọi là có

Nhấn **`Option+Space`** để gọi cửa sổ tìm kiếm bất cứ lúc nào (đổi phím được), con trỏ tự nhảy vào ô tìm — gọi là gõ, xong là đi. Hỗ trợ tự chạy khi mở máy (chạy nền thu gọn), không làm phiền workflow.

### Trải nghiệm gõ thông minh

- **Ghost text tự hoàn thành** — vừa gõ vừa hiện gợi ý mờ, từ lịch sử tìm kiếm (xếp theo tần suất) hoặc từ khóa hệ thống (gõ `ex` gợi ý `ext:`). Nhấn **Tab** là nhận
- **Gợi ý tên file (suggest)** — gõ vài ký tự đầu, server trả ngay các tên khớp nhất, xếp theo độ mới. Có thể giới hạn trong 1 thư mục
- **Highlight cú pháp** — tô màu realtime: tên filter màu tím, tham số màu xanh, chuỗi trong ngoặc màu cam, toán tử màu đỏ
- **Huy hiệu tùy chọn** — cạnh ô tìm kiếm, bật/tắt nhanh Regex / Case Sensitive / Whole Word / Match Filename

### Tìm trong thư mục chỉ định (scope)

Giới hạn tìm kiếm trong 1 cây thư mục, không phân biệt hoa thường, chịu được tên có dấu (cả NFC/NFD):

- App: chọn thư mục scope trước khi gõ (sắp có)
- MCP/API: tham số `scope`, ví dụ tìm `DAL-8000` trong `Bộ nhớ dùng chung`
- Kết hợp được với mọi filter khác và mọi kiểu sắp xếp

### Sắp xếp theo ngày tháng

Kết quả sắp xếp được theo ngày sửa, ngày tạo (birthtime lấy từ kernel, file cũ tự fallback = ngày sửa), tên:

- `rank` (mặc định, theo độ liên quan), `mtime_desc/asc`, `birth_desc/asc`, `name_asc`
- Mỗi kết quả trả về kèm `modTime` + `birthTime` (epoch)
- App hiển thị cột Ngày sửa/Ngày tạo, bấm để đổi thứ tự (sắp có)

### Cú pháp truy vấn kiểu Everything

Parser AST đầy đủ, 15+ filter, toán tử boolean, glob, regex. Cửa sổ trợ giúp cú pháp built-in (**Cmd+?**).

| Truy vấn | Nghĩa |
|------|------|
| `readme` | Tên file chứa "readme" |
| `*.swift` | Mọi file Swift |
| `ext:py size:>1mb` | File Python trên 1MB |
| `dm:today` | File sửa hôm nay |
| `config path:/usr` | File chứa "config" dưới `/usr` |
| `"exact phrase"` | Khớp cụm chính xác |
| `foo OR bar` | Toán tử OR |
| `case:Makefile` | Phân biệt hoa thường |
| `regex:^test_.*\.py$` | Biểu thức chính quy |
| `type:folder node_modules` | Chỉ tìm thư mục |
| `~/Documents/*.pdf` | Tilde + glob |
| `infile:TODO ext:cpp` | Tìm "TODO" trong file C++ |

<details>
<summary><b>Danh sách filter đầy đủ</b></summary>

| Filter | Nghĩa | Ví dụ |
|--------|------|------|
| `ext:` | Đuôi file | `ext:swift,h` |
| `size:` | Dung lượng | `size:>1mb`, `size:100kb-5mb` |
| `type:` | File/thư mục | `type:folder` |
| `path:` | Đường dẫn chứa | `path:Downloads` |
| `nopath:` | Loại trừ đường dẫn | `nopath:node_modules` |
| `parent:` | Thư mục cha trực tiếp | `parent:src` |
| `depth:` | Độ sâu thư mục | `depth:<3` |
| `dm:` | Ngày sửa | `dm:today`, `dm:>2024-01-01` |
| `dc:` | Ngày tạo | `dc:thisweek` |
| `da:` | Ngày truy cập | `da:last7days` |
| `len:` | Độ dài tên file | `len:>50` |
| `case:` | Phân biệt hoa thường | `case:README` |
| `regex:` | Regex | `regex:^test_` |
| `ww:` | Khớp cả từ | `ww:test` |
| `wfn:` | Khớp cả tên file | `wfn:Makefile` |
| `content:` / `infile:` | Tìm nội dung | `infile:TODO` |
| `audio:` `video:` `pic:` `doc:` `zip:` | Macro loại file | `audio:` = mọi file audio |

</details>

### Tìm toàn văn bản trong file

Gõ `infile:từ-khóa` để tìm trong nội dung file, kết quả kèm đoạn ngữ cảnh highlight từ khóa. Index Trigram tăng tốc, chỉ index lại file thay đổi. Cấu hình loại file + dung lượng tối đa trong 「cài đặt nội dung」.

### Đồng bộ realtime, không bao giờ cũ

- **Giám sát file** — FSEvents theo dõi thay đổi filesystem realtime, file mới/đổi tên/xóa hiện ngay trong kết quả
- **Khởi động 2 giai đoạn** — mở app là nạp cache đĩa (tìm được ngay), nền đuổi kịp thay đổi qua FSEvents, tìm kiếm không chờ đợi
- **Tiết kiệm pin theo focus** — cửa sổ không ở foreground thì dừng refresh, quay lại thì đuổi kịp hàng loạt, CPU nền gần như bằng 0

### Chi tiết tương tác

- **Highlight thông minh** — phần khớp trong kết quả được bôi ▪, nhận biết AST: xử lý đúng glob, regex, hoa thường, NOT loại trừ
- **Kéo thả** — kéo thẳng file từ kết quả sang Finder, VS Code, Xcode, app bất kỳ
- **Chuột phải** — Open / Reveal in Finder / Copy Path
- **Cmd+Click** — định vị nhanh trong Finder
- **File gần đây** — ô tìm trống thì tự hiện file mới sửa

### Tích hợp AI (MCP)

Server [Model Context Protocol](https://modelcontextprotocol.io/) built-in, cho AI coding tool tìm filesystem tức thì. Bật 1 click ở menu bar, hỗ trợ **Claude Code**, **Cursor**, **Claude Desktop**.

```
Claude Code / Cursor / Claude Desktop
       │
       ▼  (stdio JSON-RPC 2.0)
  MacEverythingMCP
       │
       ▼  (HTTP localhost:19860)
  MacEverything.app
```

| Tool | Nghĩa |
|------|------|
| `search_files` | Tìm tên file/thư mục (tăng tốc Trigram). Có `scope`, `sort` |
| `suggest` | Gợi ý tên theo prefix, mới nhất trước. Có `scope` |
| `search_content` | Tìm toàn văn, trả đường dẫn + đoạn ngữ cảnh |
| `recent_files` | File mới sửa |
| `index_status` | Thống kê + sức khỏe index |

Tham số `sort`: `rank` (mặc định), `mtime_desc`, `mtime_asc`, `birth_desc`, `birth_asc`, `name_asc`.

### HTTP API

REST API local ở `localhost:19860`, tiện cho script và tự động hóa:

```bash
curl "http://localhost:19860/api/search?q=readme&limit=10"       # tìm file
curl "http://localhost:19860/api/search?q=readme&scope=/Users/me/Documents&sort=mtime_desc"
curl "http://localhost:19860/api/suggest?prefix=LP1&limit=5"     # gợi ý
curl "http://localhost:19860/api/search/content?q=TODO"           # tìm nội dung
curl "http://localhost:19860/api/recent?limit=20"                 # file gần đây
curl "http://localhost:19860/api/status"                          # trạng thái index
```

### Cài đặt

#### Tải DMG (khuyên dùng)

1. Tải `MacEverything.dmg` ở [Releases](../../releases)
2. Kéo `MacEverything.app` vào thư mục 「Ứng dụng」
3. Mở app, cấp **Full Disk Access** khi được hỏi
4. Chờ quét lần đầu (~14 giây)
5. Nhấn `Option+Space` để tìm

#### Build từ source

**Yêu cầu:** macOS 13+, Xcode 15+

```bash
git clone https://github.com/user/MacEverything.git && cd MacEverything

xcodebuild -project MacEverything.xcodeproj -scheme MacEverything \
  -configuration Release build SYMROOT=build

hdiutil create -volname MacEverything \
  -srcfolder build/Release/MacEverything.app \
  -ov -format UDZO MacEverything.dmg
```

#### Daemon CLI

Chế độ headless, cho server hoặc tự động hóa:

```bash
make daemon
./maceverything-daemon --port 19860 --root /
```

---

<h2 align="center">Dành cho developer: chiều sâu kỹ thuật</h2>

<p align="center">
  Phần dưới dành cho ai quan tâm chi tiết cài đặt.
</p>

### Tổng quan kiến trúc

┌─────────────────────────────────────┐
│       Tầng app SwiftUI              │  Giao diện · ViewModel · MVVM
├─────────────────────────────────────┤
│    Tầng bridge Objective-C++        │  Tương tác không overhead
├─────────────────────────────────────┤
│       Engine C++20                  │  Quét · index · tìm · persist
└─────────────────────────────────────┘
```

Cùng 1 engine C++20 chạy 3 chế độ triển khai:

| Chế độ | Nghĩa |
|------|------|
| **App GUI** | App menu bar SwiftUI, phím tắt toàn cục `Option+Space` |
| **CLI daemon** | `maceverything-daemon` headless — cùng engine, không UI |
| **Server MCP** | `MacEverythingMCP` — proxy stdio JSON-RPC cho AI tool gọi |

### Engine lõi

| Thành phần | Thiết kế chính |
|------|---------|
| **DirectoryScanner** | Work-stealing đa luồng + `getattrlistbulk` lấy thuộc tính hàng loạt (kể cả giờ tạo) trong 1 syscall, 4–32 luồng tự thích ứng |
| **SearchEngine** | Index đảo Trigram (2 index name + path) + chọn tập ứng viên tối ưu + lọc cột SoA |
| **ContentIndex** | Index đảo toàn văn Trigram, cập nhật tăng dần hash FNV-1a, chỉ index lại file đổi |
| **SIMDSearch** | So khớp vector NEON ARM 128-bit first-last byte + unroll 2x, đơn luồng 11.5 GB/s |
| **IndexPersistence** | WAL + CRC32 + flush dirty-page + rename nguyên tử, nén COW không chặn (giữ lock < 100ms). Flat v6 section hóa, Paged v5, legacy V4 — section/cột mới tương thích 2 chiều |
| **FileSystemWatcher** | FSEvents + phát lại tăng dần theo eventId + phát hiện cắt log tự quét lại subtree |
| **PathTable** | Intern chuỗi đường dẫn — mỗi thư mục chỉ 1 `uint32`, triệu file tiết kiệm ~550MB |
| **QueryParser** | Pipeline AST đầy đủ: Tokenizer → FilterParser → Parser → QueryAST, 30+ từ khóa filter |

### Benchmark

Môi trường: macOS Darwin 24.3.0, **5.4 triệu file index**, 48 loại truy vấn:

#### Độ trễ tìm kiếm

| Loại truy vấn | Trễ trung bình | Ví dụ |
|----------|:---------:|------|
| Từ khóa dài (7+ ký tự) | **0.1–1ms** | `screenshot` 0.1ms, `dockerfile` 0.1ms |
| Từ khóa vừa (4–6 ký tự) | **1–5ms** | `readme` 1.2ms, `config` 4.7ms |
| Glob | **0.7–18ms** | `*.cpp` 0.7ms, `*.swift` 1.5ms |
| Truy vấn đường dẫn | **3–32ms** | `package.json` 2.9ms |
| Trung bình 48 loại | **10.5ms** | Kết quả mới nhất sau tối ưu SoA |

#### Trigram vs quét tuyến tính

| Truy vấn | Trigram | Tuyến tính | Tăng tốc |
|------|:------:|:------:|:------:|
| `node_modules` | 0.5ms | 154ms | **308x** |
| `application` | 2.1ms | 175ms | **83x** |
| `readme` | 1.2ms | 49ms | **41x** |

#### Tìm chuỗi SIMD (Apple M3 Pro)

| Phương pháp | Thông lượng | So với `std::string::find` |
|------|:------:|:------------------------:|
| `std::string::find` | 1.2 GB/s | Baseline |
| **NEON 128-bit (đơn luồng）** | **11.5 GB/s** | **9.5x** |
| **NEON 128-bit (12 luồng）** | **74.3 GB/s** | **60.7x** |

### Kỹ thuật then chốt

| Kỹ thuật | Hiệu quả |
|------|------|
| `getattrlistbulk` | 1 syscall lấy thuộc tính hàng loạt — khỏi `stat` từng file |
| Index đảo Trigram | Tìm gần tuyến tính: nhanh hơn quét tuyến tính 33x–308x |
| Bố cục cột SoA | Truy cập thân thiện cache, truy vấn lọc thuần SIMD 16 record/lần |
| `__builtin_prefetch` | Prefetch khoảng cách 8, giấu độ trễ RAM ở phase xác minh |
| ARM NEON SIMD | So chuỗi vector 128-bit, unroll 2x, gần kịch băng thông RAM |
| GCD quét song song | Trigram không gánh được thì dùng đa nhân quét tuyến tính |
| StringPool liền mạch | Tên file xếp gọn trong 1 buffer `char`, thân thiện SIMD |
| Intern PathTable | Đường dẫn chỉ lưu `uint32` — triệu file tiết kiệm ~550MB |
| Bộ đếm Generation | Mỗi 1024 vòng lặp check 1 lần, gõ nhanh hủy query cũ zero-cost |
| Khử trùng Firmlink APFS | inode + devid phát hiện, xử lý đúng vòng merge Data/System |
| Prefilter Trigram cho Regex | Trích literal từ regex làm ứng viên trigram, ~7s → <100ms |
| Bypass Trigram thích ứng | Ứng viên quá đông thì về quét song song, khỏi tra index vô ích |
| Nén COW không chặn | Copy-on-write, giữ exclusive lock < 100ms (trước 30–60s) |
| Lưu trữ tăng dần phân trang | Chỉ ghi dirty page, flush I/O thường từ ~112MB xuống KB |
| Prefix scope + NFC/NFD | Lọc theo tiền tố đường dẫn, khớp cả 2 chuẩn Unicode macOS |

### Hệ thống test

81 module test phủ toàn stack, hỗ trợ AddressSanitizer và ThreadSanitizer:

```bash
make test          # unit test nhanh + lint bridge
make test-slow     # test tích hợp (quét toàn đĩa, FSEvents, end-to-end)
make test-all      # toàn bộ
make test-asan     # AddressSanitizer
make test-tsan     # ThreadSanitizer
```

Phạm vi:
- **Engine lõi**: quét, truy vấn, mutation, nén, sắp xếp, tìm đường dẫn, scope, suggest, birthtime
- **Persist**: toàn vẹn WAL CRC, replay hàng loạt, race, paged v5, flat v6 section
- **Index nội dung**: Trigram, nén, theo dõi mtime, WAL
- **Tìm/query**: tokenizer, parser, filter, lọc ngày, structured query, regex trigram, highlight hint
- **Hiệu năng**: tìm SIMD, benchmark tổng hợp chục triệu record, đua trigram
- **Tích hợp**: thread-safe, end-to-end, hot-swap engine qua HTTP, giao thức MCP
- **An toàn bộ nhớ**: build ASan + TSan

### Cấu trúc project

```
MacEverything/
├── Core/                  # Engine C++20
│   ├── SearchEngine       # Index Trigram + truy vấn song song (6 file .cpp)
│   ├── DirectoryScanner   # Scanner hàng loạt đa luồng
│   ├── ContentIndex       # Index đảo toàn văn
│   ├── IndexPersistence   # WAL + persist phân trang
│   ├── FileSystemWatcher  # Giám sát realtime FSEvents
│   ├── HttpServer         # Server REST API nhúng
│   ├── SIMDSearch         # Tìm chuỗi vector NEON ARM
│   ├── QueryAST/Parser    # Pipeline ngôn ngữ truy vấn
│   ├── PathTable          # Bảng intern chuỗi
│   └── ServiceEngine      # Điều phối vòng đời
├── Bridge/                # Tầng bridge Objective-C++
│   └── MacSearchBridge    # Tương tác C++ ↔ Swift zero-cost
├── App/                   # Tầng app SwiftUI
│   ├── ContentView        # Giao diện tìm chính
│   ├── SearchViewModel    # MVVM + debounce phân tầng
│   ├── HotkeyManager      # Đăng ký phím tắt toàn cục
│   └── MCPConfigManager   # Cấu hình MCP 1 click
├── CLI/                   # Tool dòng lệnh
│   ├── daemon_main        # Daemon headless
│   └── mcp_main           # Server MCP (stdio JSON-RPC)
└── tests/                 # 81 module test
```

## Đóng góp

Hoan nghênh đóng góp! Quy trình:

1. Fork repo
2. Tạo nhánh tính năng (`feat/...`) hoặc sửa lỗi (`fix/...`)
3. Viết test cho tính năng mới
4. Đảm bảo `make test-all` xanh
5. Gửi Pull Request

## Giấy phép

Project open source theo giấy phép MIT — xem file [LICENSE](LICENSE).

---

<p align="center">
  <b>Nếu MacEverything giúp anh tìm file nhanh hơn, cho 1 Star ủng hộ nhé!</b>
</p>
