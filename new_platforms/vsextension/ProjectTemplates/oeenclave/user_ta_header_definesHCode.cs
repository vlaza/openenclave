using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OpenEnclaveSDK.ProjectTemplates.oeenclave
{
    public partial class user_ta_header_definesH
    {
        private Guid m_guid1;
        public user_ta_header_definesH(String guid1) { this.m_guid1 = Guid.Parse(guid1); }
        public String Guid1String => m_guid1.ToString();
        public String Guid1Struct => m_guid1.ToString("X");
    }
}
