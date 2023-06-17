/*! \file code-object.hxx
 *
 * An abstract interface to mediate between the object files generated by
 * LLVM and the SML/NJ in-memory code objects.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2021 The Fellowship of SML/NJ (http://www.smlnj.org)
 * All rights reserved.
 */

#ifndef _CODE_OBJECT_HXX_
#define _CODE_OBJECT_HXX_

#include <vector>
#include "llvm/Object/ObjectFile.h"

struct target_info;

//! a code-object is container for the parts of an object file that are needed to
//! create the SML code object in the heap.  Its purpose is to abstract from
//! target architecture and object-file format dependencies.  This class is
//! an abstract base class; the actual implementation is specialized to the
//! target.
//
class CodeObject {
  public:

    CodeObject () = delete;
    CodeObject (CodeObject &) = delete;

    virtual ~CodeObject ();

    //! create a code object.
    static std::unique_ptr<CodeObject> create (class code_buffer *codeBuf);

    //! return the size of the code in bytes
    size_t size() const { return this->_szb; }

    //! copy the code into the specified memory, which is assumed to be this->size()
    //! bytes
    void getCode (unsigned char *code);

    //! dump information about the code object to the LLVM debug stream.
    void dump (bool bits);

  protected:
    const target_info *_tgt;
    std::unique_ptr<llvm::object::ObjectFile> _obj;
    size_t _szb;
    std::vector<llvm::object::SectionRef> _sects;

    CodeObject (
	target_info const *target,
	std::unique_ptr<llvm::object::ObjectFile> objFile
    ) : _tgt(target), _obj(std::move(objFile)), _szb(0)
    { }

  //! helper function that determines which sections to include and computes
  //! the total size of the SML code object
  // NOTE: because this function invokes the target-specific virtual method
  // `_includeDataSect`, it must be called *after* the object has been
  // constructed.
  //
    void _computeSize ();

  //! should a section be included in the SML data object?
  //
    bool _includeSect (llvm::object::SectionRef &sect)
    {
	return sect.isText() || (sect.isData() && this->_includeDataSect(sect));
    }

  //! should a data section be included in the code object?  This method
  //! is target specific.
  //
    virtual bool _includeDataSect (llvm::object::SectionRef &sect) = 0;

  //! helper function for resolving relocation records
  //
    virtual void _resolveRelocs (llvm::object::SectionRef &sect, uint8_t *code) = 0;

  /// dump the relocation info for a section
  //
    void _dumpRelocs (llvm::object::SectionRef &sect);

}; // CodeObject

#endif //! _CODE_OBJECT_HXX_
