#include <fstream>

#include <libasr/assert.h>
#include <libasr/codegen/wasm_to_wat.h>

#define WAT_DEBUG

#ifdef WAT_DEBUG
#define DEBUG(s) std::cout << s << std::endl
#else
#define DEBUG(s)
#endif

namespace LFortran {

namespace wasm {

void WASMDecoder::decode_type_section(uint32_t offset) {
    // read type section contents
    uint32_t no_of_func_types = read_unsigned_num(wasm_bytes, offset);
    DEBUG("no_of_func_types: " + std::to_string(no_of_func_types));
    func_types.reserve(al, no_of_func_types);

    for (uint32_t i = 0; i < no_of_func_types; i++) {        
        if (wasm_bytes[offset] != 0x60) {
            std::cout << "Error: Invalid type section" << std::endl;
            exit(1);
        }
        offset++;

        FuncType func_type;
        // read result type 1
        uint32_t no_of_params = read_unsigned_num(wasm_bytes, offset);
        func_type.param_types.reserve(al, no_of_params);

        for (uint32_t j = 0; j < no_of_params; j++) {
            func_type.param_types.push_back(al, read_byte(wasm_bytes, offset));
        }

        // read result type 2
        uint32_t no_of_results = read_unsigned_num(wasm_bytes, offset);
        func_type.result_types.reserve(al, no_of_results);

        for (uint32_t j = 0; j < no_of_results; j++) {
            func_type.result_types.push_back(al, read_byte(wasm_bytes, offset));
        }

        func_types.push_back(al, func_type);
    }
}

void WASMDecoder::decode_function_section(uint32_t offset) {
    // read function section contents
    uint32_t no_of_indices = read_unsigned_num(wasm_bytes, offset);
    DEBUG("no_of_indices: " + std::to_string(no_of_indices));
    type_indices.reserve(al, no_of_indices);

    for (uint32_t i = 0; i < no_of_indices; i++) {
        type_indices.push_back(al, read_unsigned_num(wasm_bytes, offset));
    }
}

void WASMDecoder::decode_export_section(uint32_t offset) {
    // read export section contents
    uint32_t no_of_exports = read_unsigned_num(wasm_bytes, offset);
    DEBUG("no_of_exports: " + std::to_string(no_of_exports));
    exports.reserve(al, no_of_exports);

    for (uint32_t i = 0; i < no_of_exports; i++) {
        Export expt;
        uint32_t name_size = read_unsigned_num(wasm_bytes, offset);
        expt.name.resize(name_size);
        for (uint32_t j = 0; j < name_size; j++) {
            expt.name[j] = read_byte(wasm_bytes, offset);
        }
        DEBUG("export name: " + expt.name);
        expt.kind = read_byte(wasm_bytes, offset);
        DEBUG("export kind: " + std::to_string(expt.kind));
        expt.index = read_unsigned_num(wasm_bytes, offset);
        DEBUG("export index: " + std::to_string(expt.index));

        exports.push_back(al, expt);
    }
}

void WASMDecoder::decode_code_section(uint32_t offset) {
    // read code section contents
    uint32_t no_of_codes = read_unsigned_num(wasm_bytes, offset);
    DEBUG("no_of_codes: " + std::to_string(no_of_codes));
    codes.reserve(al, no_of_codes);

    for (uint32_t i = 0; i < no_of_codes; i++) {
        Code code;

        code.size = read_unsigned_num(wasm_bytes, offset);
        uint32_t code_start_offset = offset;
        uint32_t no_of_locals = read_unsigned_num(wasm_bytes, offset);
        DEBUG("no_of_locals: " + std::to_string(no_of_locals));
        code.locals.reserve(al, no_of_locals);

        DEBUG("Entering loop");
        for (uint32_t j = 0U; j < no_of_locals; j++) {
            Local local;
            local.count = read_unsigned_num(wasm_bytes, offset);
            DEBUG("count: " + std::to_string(local.count));
            local.type = read_byte(wasm_bytes, offset);
            DEBUG("type: " + std::to_string(local.type));
            code.locals.push_back(al, local);
        }
        DEBUG("Exiting loop");

        code.insts_start_index = offset;
        codes.push_back(al, code);
        // skip offset to directly the end of instructions
        offset = code_start_offset + code.size;
    }
}

void WASMDecoder::decode_wasm() {
    // first 8 bytes are magic number and wasm version number
    // currently, in this first version, we are skipping them
    uint32_t index = 8U;

    for(auto &i:byte_kind_to_string){
        std::cout << i.second << std::endl;
    }

    for(auto &i:byte_type_to_string){
        std::cout << i.second << std::endl;
    }
    while (index < wasm_bytes.size()) {
        uint32_t section_id = read_unsigned_num(wasm_bytes, index);
        uint32_t section_size = read_unsigned_num(wasm_bytes, index);
        switch (section_id) {
            case 1U:
                decode_type_section(index);
                // exit(0);
                break;
            case 3U:
                decode_function_section(index);
                // exit(0);
                break;
            case 7U:
                decode_export_section(index);
                // exit(0);
                break;
            case 10U:
                decode_code_section(index);
                // exit(0)
                break;
            default:
                std::cout << "Unknown section id: " << section_id << std::endl;
                break;
        }
        index += section_size;
    }

    LFORTRAN_ASSERT(index == wasm_bytes.size());
    LFORTRAN_ASSERT(type_indices.size() == codes.size());
}

void WASMDecoder::load_file(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    wasm_bytes.reserve(al, size);
    file.read((char*)wasm_bytes.data(), size);
    file.close();
}

std::string WASMDecoder::get_wat() {
    std::cout << "----------------- DEBUGGING ------------------" << std::endl;
    std::cout << func_types.size() << std::endl;
    std::cout << func_types[1].param_types.size() << std::endl;
    std::cout << func_types[1].result_types.size() << std::endl;

    for(auto &i:byte_kind_to_string){
        std::cout << i.first << ", " << i.second << std::endl;
    }

    for(auto &i:byte_type_to_string){
        std::cout << i.first << ", " << i.second << std::endl;
    }

    std::string result = "(module";
    for (uint32_t i = 0; i < type_indices.size(); i++) {
        result += "\n    (func $" + std::to_string(i);
        result += "\n        (param";
        uint32_t func_index = type_indices[i];
        for (uint32_t j = 0; j < func_types[func_index].param_types.size(); j++) {
            std::cout << "Printing param value: " << func_types[func_index].param_types[j] << std::endl;
            result += " " + byte_type_to_string[func_types[func_index].param_types[j]];
        }
        result += ") (result";
        for (uint32_t j = 0; j < func_types[func_index].result_types.size(); j++) {
            result += " " + byte_type_to_string[func_types[func_index].result_types[j]];
        }
        result += ")";
        result += "\n        (local";
        for (uint32_t j = 0; j < codes[i].locals.size(); j++) {
            for (uint32_t k = 0; k < codes[i].locals[j].count; k++) {
                result += " " + byte_type_to_string[codes[i].locals[j].type];
            }
        }
        result += ")";

        {
            WASM_INSTS_VISITOR::WATVisitor v = WASM_INSTS_VISITOR::WATVisitor();
            v.indent = "\n        ";
            v.decode_instructions(wasm_bytes, codes[i].insts_start_index);
            result += v.src;
        }

        result += "\n    )\n";
    }

    for (uint32_t i = 0; i < exports.size(); i++) {
        result += "\n    (export \"" + exports[i].name + "\" (" + byte_kind_to_string[exports[i].kind] + " $" + std::to_string(exports[i].index) + "))";
    }
    result += "\n)";

    return result;
}

}  // namespace wasm

}  // namespace LFortran
