let project_dir = '/home/joobog/git/predictor/'
let build_dir = project_dir . 'build/'

exe 'CMakeProjectDir' project_dir
exe 'CMakeBuildLocation' build_dir
exe 'MakeFile' build_dir .'Makefile'
exe 'MakeCmdlineArgs -j4'


exe 'DoxygenConfigFile' project_dir .'Doxyfile.config'
exe 'DoxygenLogFile' project_dir .'Doxyfile.log'
exe 'DoxygenWarningFile' project_dir .'Doxyfile.warning'

let command = build_dir . '../run/run.sh'
exe "nnoremap <leader>t :!" . command . " <CR>"
