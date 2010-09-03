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

/// <summary>Abstract base class for the cryptographic documents.</summary>
public class BEID_Crypto : BEID_XMLDoc {
  private HandleRef swigCPtr;

  internal BEID_Crypto(IntPtr cPtr, bool cMemoryOwn) : base(beidlib_dotNetPINVOKE.BEID_CryptoUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new HandleRef(this, cPtr);
  }

  internal static HandleRef getCPtr(BEID_Crypto obj) {
    return (obj == null) ? new HandleRef(null, IntPtr.Zero) : obj.swigCPtr;
  }

  ~BEID_Crypto() {
    Dispose();
  }

  public override void Dispose() {
    lock(this) {
      if(swigCPtr.Handle != IntPtr.Zero && swigCMemOwn) {
        swigCMemOwn = false;
        beidlib_dotNetPINVOKE.delete_BEID_Crypto(swigCPtr);
      }
      swigCPtr = new HandleRef(null, IntPtr.Zero);
      GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

}

}
