var gulp = require('gulp');
var gzip = require('gulp-gzip');

gulp.task('compress', async function() {
  gulp.src(['../data/**/*.*'])
      .pipe(gzip())
      .pipe(gulp.dest('../data/gzip'));
});
