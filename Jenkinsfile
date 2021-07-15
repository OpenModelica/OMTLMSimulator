pipeline {
  agent {
    docker {
      image 'openmodelica/build-deps'
    }

  }
  stages {
    stage('build') {
      steps {
        sh 'ABI=LINUX64 make'
      }
    }
  }
}