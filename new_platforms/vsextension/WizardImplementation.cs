using System;
using System.Collections.Generic;
using Microsoft.VisualStudio.TemplateWizard;
using System.Windows.Forms;
using EnvDTE;
using OpenEnclaveSDK.ProjectTemplates.oeenclave;
using Microsoft.VisualStudio.TextTemplating;
using Microsoft.VisualStudio.TextTemplating.VSHost;
using System.IO;

namespace OpenEnclaveSDK
{
    public class WizardImplementation : IWizard
    {
        // This method is called before opening any item that   
        // has the OpenInEditor attribute.  
        public void BeforeOpeningFile(ProjectItem projectItem)
        {
        }

        public void ProjectFinishedGenerating(Project project)
        {
        }

        // This method is only called for item templates, not for project templates.  
        public void ProjectItemFinishedGenerating(ProjectItem projectItem)
        {
        }

        // This method is called after the project is created.  
        public void RunFinished()
        {
        }

        private static void WriteFile(string outputFolder, string fileName, string contents)
        {
            using (var file = new StreamWriter(Path.Combine(outputFolder, fileName)))
            {
                file.Write(contents);
            }
        }

        public void RunStarted(
            object automationObject,
            Dictionary<string, string> replacementsDictionary,
            WizardRunKind runKind,
            object[] customParams)
        {
            try
            {
                string destinationDirectory;
                replacementsDictionary.TryGetValue("$destinationdirectory$", out destinationDirectory);
                string opteeDirectory = Path.Combine(destinationDirectory, "optee");

                string guid1;
                replacementsDictionary.TryGetValue("$guid1$", out guid1);

                var pt1 = new user_ta_header_definesH(guid1);
                string outputText1 = pt1.TransformText();
                WriteFile(opteeDirectory, "user_ta_header_defines.h", outputText1);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        // This method is only called for item templates, not for project templates.  
        public bool ShouldAddProjectItem(string filePath)
        {
            return true;
        }
    }
}
