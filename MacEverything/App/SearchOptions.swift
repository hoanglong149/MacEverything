import SwiftUI
import Combine

@MainActor
class SearchOptions: ObservableObject {
    static let shared = SearchOptions()

    @Published var isRegex: Bool = false {
        didSet {
            if isRegex {
                isWholeWord = false
                isMatchFilename = false
            }
        }
    }
    @Published var isCaseSensitive: Bool = false
    @Published var isWholeWord: Bool = false {
        didSet {
            if isWholeWord {
                isRegex = false
                isMatchFilename = false
            }
        }
    }
    @Published var isMatchFilename: Bool = false {
        didSet {
            if isMatchFilename {
                isRegex = false
                isWholeWord = false
            }
        }
    }

    enum SortOption: Int, CaseIterable {
        case rank = 0, mtimeDesc, mtimeAsc, birthDesc, birthAsc, nameAsc
        var label: String {
            switch self {
            case .rank: return "Relevance"
            case .mtimeDesc: return "Modified ↓"
            case .mtimeAsc: return "Modified ↑"
            case .birthDesc: return "Created ↓"
            case .birthAsc: return "Created ↑"
            case .nameAsc: return "Name A–Z"
            }
        }
    }

    @Published var sortOption: SortOption = .rank {
        didSet { UserDefaults.standard.set(sortOption.rawValue, forKey: "sortOption") }
    }
    @Published var showSize: Bool = true {
        didSet { UserDefaults.standard.set(showSize, forKey: "showSize") }
    }
    @Published var showModified: Bool = true {
        didSet { UserDefaults.standard.set(showModified, forKey: "showModified") }
    }
    @Published var showCreated: Bool = false {
        didSet { UserDefaults.standard.set(showCreated, forKey: "showCreated") }
    }

    private init() {
        let d = UserDefaults.standard
        sortOption = SortOption(rawValue: d.integer(forKey: "sortOption")) ?? .rank
        if d.object(forKey: "showSize") != nil { showSize = d.bool(forKey: "showSize") }
        if d.object(forKey: "showModified") != nil { showModified = d.bool(forKey: "showModified") }
        if d.object(forKey: "showCreated") != nil { showCreated = d.bool(forKey: "showCreated") }
    }

    var hasActiveOptions: Bool {
        isRegex || isCaseSensitive || isWholeWord || isMatchFilename
    }

    /// Build query string with prefixes for the C++ engine
    func buildQuery(_ keyword: String) -> String {
        guard !keyword.isEmpty else { return keyword }
        var prefix = ""
        if isRegex { prefix += "regex:" }
        if isCaseSensitive { prefix += "case:" }
        if isWholeWord { prefix += "ww:" }
        if isMatchFilename { prefix += "wfn:" }
        return prefix + keyword
    }
}

struct SearchOptionBadges: View {
    @ObservedObject var options: SearchOptions

    var body: some View {
        if options.hasActiveOptions {
            HStack(spacing: 4) {
                if options.isRegex {
                    badge(".*", color: .purple) { options.isRegex.toggle() }
                }
                if options.isCaseSensitive {
                    badge("Aa", color: .orange) { options.isCaseSensitive.toggle() }
                }
                if options.isWholeWord {
                    badge("W", color: .blue) { options.isWholeWord.toggle() }
                }
                if options.isMatchFilename {
                    badge("FN", color: .green) { options.isMatchFilename.toggle() }
                }
            }
        }
    }

    private func badge(_ text: String, color: Color, action: @escaping () -> Void) -> some View {
        Button(action: action) {
            Text(text)
                .font(.system(size: 11, weight: .bold, design: .monospaced))
                .foregroundColor(.white)
                .padding(.horizontal, 5)
                .padding(.vertical, 2)
                .background(RoundedRectangle(cornerRadius: 4).fill(color))
        }
        .buttonStyle(.plain)
    }
}
