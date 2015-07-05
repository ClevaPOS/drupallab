module.exports = function (grunt) {
    grunt.initConfig({
        watch: {
            src: {
                files: ['**/*.scss', '**/*.php','**/*.sass'],
                tasks: ['compass:dev','jshint']
            },
            options: {
                livereload: true,
            },
        },
        compass: {
            dev: {
                options: {
                    sassDir: 'custom-sass',
                    cssDir: 'css',
                    imagesPath: 'assets/img',
                    noLineComments: false,
                    outputStyle: 'compressed'
                }
            }
        },
        jshint: {
          files: ['Gruntfile.js', 'scripts/**/*.js'],
          options: {
            globals: {
              jQuery: true
            }
          }
        }

    });
    grunt.loadNpmTasks('grunt-contrib-compass');
    grunt.loadNpmTasks('grunt-contrib-sass');
    grunt.loadNpmTasks('grunt-contrib-watch');
    grunt.loadNpmTasks('grunt-contrib-jshint');
    grunt.registerTask('default', ['jshint']);

};

