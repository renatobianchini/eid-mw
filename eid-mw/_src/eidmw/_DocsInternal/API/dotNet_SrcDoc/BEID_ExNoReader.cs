/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.35
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

namespace be.belgium.eid {

using System;
using System.Runtime.InteropServices;

/// <summary>
/// Exception class No reader (error code = EIDMW_ERR_NO_READER).
/// Throw when the reader set is empty.
/// Used in : 
/// - BEID_Object::checkContextStillOk()
/// </summary>
public class BEID_ExNoReader : BEID_Exception {
  private HandleRef swigCPtr;

  internal BEID_ExNoReader(IntPtr cPtr, bool cMemoryOwn) : base(beidlib_dotNetPINVOKE.BEID_ExNoReaderUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(BEID_ExNoReader obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~BEID_ExNoReader() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        beidlib_dotNetPINVOKE.delete_BEID_ExNoReader(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  public BEID_ExNoReader() : this(beidlib_dotNetPINVOKE.new_BEID_ExNoReader(), true) {
  }

}

}
