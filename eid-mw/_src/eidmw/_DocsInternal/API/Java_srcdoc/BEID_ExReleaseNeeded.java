/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.35
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package be.belgium.eid;

/******************************************************************************//**
  * Exception class Release Needed (error code = EIDMW_ERR_RELEASE_NEEDED)
  *
  *	Thrown when the application closes without calling BEID_RealeaseSDK()
  *********************************************************************************/
public class BEID_ExReleaseNeeded extends BEID_Exception {
  private long swigCPtr;

  protected BEID_ExReleaseNeeded(long cPtr, boolean cMemoryOwn) {
    super(beidlibJava_WrapperJNI.SWIGBEID_ExReleaseNeededUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }

  protected static long getCPtr(BEID_ExReleaseNeeded obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      beidlibJava_WrapperJNI.delete_BEID_ExReleaseNeeded(swigCPtr);
    }
    swigCPtr = 0;
    super.delete();
  }

	/** Constructor */
  public BEID_ExReleaseNeeded() {
    this(beidlibJava_WrapperJNI.new_BEID_ExReleaseNeeded(), true);
  }

}
