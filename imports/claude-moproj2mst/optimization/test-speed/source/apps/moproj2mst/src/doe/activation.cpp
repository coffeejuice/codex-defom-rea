#include "m2m/doe_activation.hpp"

#include "m2m/doe_sample_io.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::doe {
namespace {
using qt::L;

String asciiText(const std::string& bytes)
{
    std::u16string out;
    out.reserve(bytes.size());
    for (unsigned char ch : bytes) {
        if (ch == 0 || ch >= 128)
            throw Error(M2M_E_UNSUPPORTED, "DOE PAR QTextStream codec outside recovered ASCII branch");
        if (ch != '\r') out += static_cast<char16_t>(ch); // QFile ReadOnly|Text
    }
    return String(std::move(out));
}
}  // namespace

bool loadCurrentSamples(Study& study, const String& controlDir, StudyIo& io)
{
    std::string bytes;
    if (!io.readFile(controlDir + qt::kSep + L("DEF_DOE.PAR"), bytes)) return false;
    loadCurrentSampleText(study, asciiText(bytes));
    return true;
}

void readLazyStudy(Study& study, const xml::Node& element, const String& studiesDir, StudyIo& io)
{
    readStudyElement(study, element);
    const String control = studiesDir + qt::kSep + study.name + qt::kSep + L("Control");
    const String input = control + qt::kSep + L("DoeInputTable.XML");
    std::string bytes;
    if (!io.readFile(input, bytes))
        io.setLastError(qt::arg(L("Failed to open file, %1"), input));
    else
        (void)readInputTable(bytes, study);                  // ignored parent result
    const char* const names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (int mode = 0; mode < 3; ++mode)
        (void)loadSampleTable(study, mode, control + qt::kSep + L(names[mode]), io);
    (void)loadCurrentSamples(study, control, io);
}

}  // namespace m2m::doe
