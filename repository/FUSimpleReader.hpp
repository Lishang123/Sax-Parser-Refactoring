#ifndef E2ERUNTIME_FUSIMPLEREADER_HPP
#define E2ERUNTIME_FUSIMPLEREADER_HPP

#include "../String/ST_String.hpp"
class TY_Blob;

namespace functions::repository::simple
{
    /**
     *  Represents a single function element in the repository.
     *  Each function consists of:
     *     - a unique identifier (ID)
     *     - the function source content
     */
    struct Function {
        ST_String id;
        ST_String source;
    };

    // Collection of parsed functions.
    // The order matches the order in the XML document.
    using Functions = std::vector<Function>;

    /**
     * Parse a repository simple XML blob and return all contained functions.
     *
     * Expected XML structure:
     *  <Functions>
     *   <Function>
     *        <ID>...</ID>
     *        <Source>...</Source>
     *    </Function>
     *  </Functions>
     *
     * Behavior:
     *   - Element names are treated case-insensitively.
     *   - Function IDs must be unique.
     *   - Throws on malformed structure or validation errors.
     *
     * @param data XML document as binary blob
     * @param repo string label for error reporting
     * @return parsed functions.
     */
    Functions readRepo( const TY_Blob &data, std::string_view repo );

}//namespace functions::repository::simple

#endif //E2ERUNTIME_FUSIMPLEREADER_HPP
