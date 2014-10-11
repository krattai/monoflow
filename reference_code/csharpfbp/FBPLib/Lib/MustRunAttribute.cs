using System;
using System.Collections.Generic;
using System.Text;

namespace FBPLib
{
    [AttributeUsage(AttributeTargets.Class)]
    public class MustRun : Attribute
    {
        /* *
         * Copyright 2007, 2008, J. Paul Morrison.  At your option, you may copy, 
         * distribute, or make derivative works under the terms of the Clarified Artistic License, 
         * based on the Everything Development Company's Artistic License.  A document describing 
         * this License may be found at http://www.jpaulmorrison.com/fbp/artistic2.htm. 
         * THERE IS NO WARRANTY; USE THIS PRODUCT AT YOUR OWN RISK.
         * */
        internal bool value = true;
       
    }
}
