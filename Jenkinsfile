pipeline {
	agent {
		label "built-in"
	}
	stages {
		stage("Build document") {
			steps {
				sh("doxygen")
				sh("rm -rf /var/www/milsko-doxygen")
				sh("mv doxygen/html /var/www/milsko-doxygen")
			}
		}
	}
}
