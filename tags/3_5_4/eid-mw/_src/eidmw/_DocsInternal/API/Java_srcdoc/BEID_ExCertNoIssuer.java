/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.35
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package be.belgium.eid;

/******************************************************************************//**
  * Exception class Certificate No Issuer (error code = EIDMW_ERR_CERT_NOISSUER)
  *
  *	Thrown when asked for the issuer of a root certificate
  * Used in : - BEID_Certificate::getIssuer()
  *********************************************************************************/
public class BEID_ExCertNoIssuer extends BEID_Exception {
  private long swigCPtr;

  protected BEID_ExCertNoIssuer(long cPtr, boolean cMemoryOwn) {
    super(beidlibJava_WrapperJNI.SWIGBEID_ExCertNoIssuerUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(BEID_ExCertNoIssuer obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      beidlibJava_WrapperJNI.delete_BEID_ExCertNoIssuer(swigCPtr);
    }
    swigCPtr = 0;
    super.delete();
  }

	/** Constructor */
  public BEID_ExCertNoIssuer() {
    this(beidlibJava_WrapperJNI.new_BEID_ExCertNoIssuer(), true);
  }

}